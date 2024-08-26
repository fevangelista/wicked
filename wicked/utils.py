import wicked

__all__ = ["string_to_expr", "gen_op", "gen_op_ms0", "compile_einsum", 
           "dict_to_einsum", "analyze_einsum", "precompute_path", 
           "equation_to_dict", "dict_to_equation"]

def string_to_expr(s):
    """
    This function takes a string, splits it, and converts
    it into an Expression object
    """
    expr = wicked.Expression()
    for line in s.split("\n"):
        expr += wicked.expression(line)
    return expr

def split(word):
    return [char for char in word]

def gen_op(label, rank, cre_spaces, ann_spaces, diagonal=True):
    """
    This function automates the creation of operators that span multiple spaces.

    For example, instead of specifying all the components of an operator via
    wicked.op('T',['a+ c','v+ c','v+ a'])
    one can directly specify the
    wicked.gen_op('T',1,'av','ca')
    """
    import itertools

    cre_spaces = split(cre_spaces)
    ann_spaces = split(ann_spaces)

    osi = wicked.osi()
    m = {osi.label(n): n for n in range(osi.num_spaces())}

    c = [cre_spaces for n in range(rank)]
    a = [ann_spaces for n in range(rank)]
    terms = []
    for le in itertools.product(*c):
        is_le_sorted = all(m[le[i]] <= m[le[i + 1]]
                           for i in range(len(le) - 1))
        if is_le_sorted:
            for re in itertools.product(*a):
                is_re_sorted = all(m[re[i]] >= m[re[i + 1]]
                                   for i in range(len(re) - 1))
                if is_re_sorted:
                    if not diagonal:
                        if le != re:
                            terms.append(
                                " ".join([s + "+" for s in le]) +
                                " " + " ".join(re)
                            )
                    else:
                        terms.append(
                            " ".join([s + "+" for s in le]) +
                            " " + " ".join(re)
                        )
    return wicked.op(label, terms, unique=False)

def gen_op_ms0(label, rank, cre_spaces, ann_spaces, diagonal=True):
    """
    This function automates the creation of operators that span multiple spaces.

    The operator is for Ms=0 spin-integrated theories, such that it only contains spin-preserving terms.

    For example, instead of specifying all the components of an operator via
    wicked.op('T',['a+ c','v+ c','v+ a','A+ C','V+ C','V+ A'])
    one can directly specify the
    wicked.gen_op_ms0('T',1,'av','ca')
    """
    import itertools
    cre_spaces_alpha = [_ + '+' for _ in split(cre_spaces)]
    cre_spaces_beta = [_.upper() + '+' for _ in split(cre_spaces)]
    ann_spaces_alpha = [_ for _ in split(ann_spaces)]
    ann_spaces_beta = [_.upper() for _ in split(ann_spaces)]

    terms = []
    for nalpha in range(rank+1):
        nbeta = rank - nalpha
        cre = (cre_spaces_alpha,)*nalpha + (cre_spaces_beta,)*nbeta
        ann = (ann_spaces_alpha,)*nalpha + (ann_spaces_beta,)*nbeta
        for i in itertools.product(*cre, *ann):
            term = ' '.join(i)
            if (not diagonal):
                term_cleaned = term.replace('+','').replace(' ','').lower()
                if (len(set(term_cleaned)) > 1): terms.append(term)
            else: terms.append(term)

    return wicked.op(label, terms, unique=True)

def dict_to_einsum(eq_dict, optimize="'optimal'"):
    lhs = eq_dict['lhs'][0][0]
    if eq_dict['lhs'][0][1] != '':
        lhs += "[\'" + eq_dict['lhs'][0][1] + "\']"
    rhs = "np.einsum(\'"
    indices = ''
    for t in eq_dict['rhs']:
        indices += t[2] + ','
    indices = indices[:-1]
    blocks = ''
    for t in eq_dict['rhs']:
        blocks += t[0] + "[\'" + t[1] + "\'],"
    blocks = blocks[:-1]
    rhs += indices + "->" + eq_dict['lhs'][0][2] + "\'" + ',' + blocks + f",optimize={optimize})"
    return lhs + ' += ' + f"{float(eq_dict['factor']):+.8f}" + ' * ' + rhs

def compile_einsum(equation, keys=None, return_eq_dict=False, optimize="'optimal'"):
    """
    Compile an equation into a valid einsum expression.
    Turns a Wick&d equation (wicked._wicked.Equation) like H^{c0,a0}_{a1,a2} += 1/4 T2^{c0,a0}_{a3,a4} V^{a5,a6}_{a1,a2} eta1^{a4}_{a6} eta1^{a3}_{a5}
    into the einsum code string "H_caaa += +0.25000000 * np.einsum('iuvw,xyzr,wr,vz->iuxy', T2['caaa'], V['aaaa'], eta1, eta1, optimize='optimal')"
    """
    def _get_unique_tensor_indices(tensor, unused_indices, index_dict):
        indstr = ''
        for i in tensor.upper()+tensor.lower():
            i = str(i)
            if index_dict.get(i):
                indstr += index_dict[i]
            else:
                try:
                    index = unused_indices[i[0]].pop(0)
                except:
                    raise ValueError(f"Index {i} is not available for einsum contraction. Try enlarging the index pool.")
                index_dict[i] = index
                indstr += index

        return indstr

    eq_dict = {'factor':0.0, 'lhs':[], 'rhs':[]}

    osi = wicked.osi()
    unused_indices = osi.to_dict()
    index_dict = {}

    lhs = equation.lhs()
    rhs = equation.rhs()
    factor = equation.rhs_factor()
    eq_dict['factor'] = factor

    # holds the expression part of the einsum contraction, e.g., iuvw,xyzr,wr,vz->iuxy
    index_string = ''
    # holds the tensor label part of the einsum contraction, e.g., T2['caaa'], V['aaaa'], eta1, eta1, optimize='optimal')
    tensor_label_string = ''

    for t in rhs.tensors():
        tensor_label = ''.join([str(_)[0] for _ in t.upper()]) + ''.join([str(_)[0] for _ in t.lower()])
        tensor_label_string += t.label() + "[\"" + tensor_label + "\"],"
        tensor_indx = _get_unique_tensor_indices(t, unused_indices, index_dict)
        index_string += tensor_indx
        eq_dict['rhs'].append([t.label(), tensor_label, tensor_indx])
        index_string += ','
        if (type(keys) == dict):
            if t.label() in keys:
                keys[t.label()].add(f'{tensor_label}')
            else:
                keys[t.label()] = set([tensor_label])

    index_string = index_string[:-1] + '->'
    tensor_label_string += f"optimize={optimize})"

    # If the lhs is not a scalar
    if (lhs.tensors()[0].upper() != [] or lhs.tensors()[0].lower() != []):
        ltensor = lhs.tensors()[0].label()
        left = ltensor
        left_label = ''.join([osi.label(_.space()) for _ in lhs.tensors()[0].upper()]) + \
            ''.join([osi.label(_.space()) for _ in lhs.tensors()[0].lower()])
        left += f"[\'{left_label}\']"
        res_indx = _get_unique_tensor_indices(
            lhs.tensors()[0], unused_indices, index_dict)
        index_string += res_indx
        eq_dict['lhs'].append([lhs.tensors()[0].label(), left_label, res_indx])

        if (type(keys) == dict):
            if ltensor in keys:
                keys[ltensor].add(f'{left_label}')
            else:
                keys[ltensor] = set([left_label])
    else:  # If it's scalar, just return the label.
        left = lhs.tensors()[0].label()
        eq_dict['lhs'].append([lhs.tensors()[0].label(), '', ''])

    einsum_string = left \
        + ' ' \
        + f"+= {float(factor):+.8f} * np.einsum('"\
        + index_string \
        + "', " \
        + tensor_label_string

    if return_eq_dict:
        return einsum_string, eq_dict
    else:
        return einsum_string

def analyze_einsum(einsum_str, root_index=None):
    # just get the lhs of the einsum contraction. e.g., mnab,abef
    contraction = einsum_str.split("->")[0].split("(")[1][1:]
    # put these indices into a set
    indices = contraction.split(",")
    ind_set = set(''.join(indices))
    if root_index is not None:
        ind_set = ind_set - set(root_index)
    # get the orbital space corresponding to each index
    osi = wicked.osi().to_dict()
    index_dict = {}
    for key in osi.keys():
        for index in osi[key]:
            index_dict[index] = key
    # now count the scaling
    scaling = {_:0 for _ in osi.keys()}
    for i in ind_set:
        scaling[index_dict[i]] += 1
    return scaling

def precompute_path(line, sizes_dict, memory_limit=None, optimize='optimal'):
    import opt_einsum as oe
    import re

    pathgen = oe.paths.optimal
    if optimize == 'optimal':
        pathgen = oe.paths.optimal
    elif optimize == 'greedy':
        pathgen = oe.paths.greedy
    elif optimize == 'branch_all':
        pathgen = oe.paths.branch_all
    elif optimize == 'branch_1':
        pathgen = oe.paths.branch_1
    elif optimize == 'branch_2':
        pathgen = oe.paths.branch_2

    def _precompute(line, sizes_dict, memory_limit, pathgen):
        contr = line.split("->")[0].split("(")[1][1:]
        rhs = line.split("->")[1].split("',")[0]
        lhs = contr.split(',')
        return pathgen(lhs, [rhs], sizes_dict, memory_limit)

    if 'np.einsum' in line:
        try:
            path = ['einsum_path'] + _precompute(line, sizes_dict, memory_limit, pathgen)
            return re.sub(r"optimize=.*\)", f"optimize={str(path)})", line)
        except:
            return line
    else:
        return line
    
def equation_to_dict(eq):
    res = {}
    res['factor'] = eq.rhs_factor()

    lhs = eq.lhs().tensors()[0]
    res['lhs'] = [lhs.label(), [str(_) for _ in lhs.lower()], [str(_) for _ in lhs.upper()]]

    rhs = eq.rhs()
    res['rhs'] = []
    for i in rhs.tensors():
        res['rhs'].append([i.label(), [str(_) for _ in i.lower()], [str(_) for _ in i.upper()]])
    
    return res

def dict_to_equation(eqdict):
    lhs = wicked.SymbolicTerm()
    lhs.add(wicked.tensor(*eqdict['lhs'], wicked.sym.none))
    rhs = wicked.SymbolicTerm()
    for i in eqdict['rhs']:
        rhs.add(wicked.tensor(*i, wicked.sym.none))

    return wicked.Equation(lhs, rhs, eqdict['factor'])