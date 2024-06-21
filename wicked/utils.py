import wicked

__all__ = ["string_to_expr", "gen_op",
           "compile_einsum", "compile_sigma_vector", "generate_sigma_build"]


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
    wicked.op('T',1,'av','ca')
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


def compile_einsum(equation, keys=None):
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
                index = unused_indices[i[0]].pop(0)
                index_dict[i] = index
                indstr += index

        return indstr

    osi = wicked.osi()
    unused_indices = osi.to_dict()
    index_dict = {}

    lhs = equation.lhs()
    rhs = equation.rhs()
    factor = equation.rhs_factor()

    # holds the expression part of the einsum contraction, e.g., iuvw,xyzr,wr,vz->iuxy
    index_string = ''
    # holds the tensor label part of the einsum contraction, e.g., T2['caaa'], V['aaaa'], eta1, eta1, optimize='optimal')
    tensor_label_string = ''

    for t in rhs.tensors():
        tensor_label = ''.join([str(_)[0] for _ in t.upper()]) + ''.join([str(_)[0] for _ in t.lower()])
        tensor_label_string += t.label() + "[\"" + tensor_label + "\"],"
        index_string += _get_unique_tensor_indices(t, unused_indices, index_dict)
        index_string += ','
        if (type(keys) == dict):
            if t.label() in keys:
                keys[t.label()].add(f'{tensor_label}')
            else:
                keys[t.label()] = set([tensor_label])

    index_string = index_string[:-1] + '->'
    tensor_label_string += "optimize='optimal')"

    # If the lhs is not a scalar
    if (lhs.tensors()[0].upper() != [] or lhs.tensors()[0].lower() != []):
        left = rhs.tensors()[0].label()
        left += ''.join([osi.label(_.space()) for _ in lhs.tensors()[0].upper()]) + \
            ''.join([osi.label(_.space()) for _ in lhs.tensors()[0].lower()])
        res_indx = _get_unique_tensor_indices(
            rhs.tensors(), unused_indices, index_dict)
        index_string += res_indx
    else:  # If it's scalar, just return the label.
        left = lhs.tensors()[0].label()

    einsum_string = left \
        + ' ' \
        + f"+= {float(factor):+.8f} * np.einsum('"\
        + index_string \
        + "', " \
        + tensor_label_string

    return einsum_string


def compile_sigma_vector(equation, bra_name='bra', ket_name='ket', n_lowest=1):
    """
    Compile an equation into a valid einsum expression for sigma vector.
    Turns a Wick&d equation (wicked._wicked.Equation) like H^{c0,a0}_{a1,a2} += 1/4 T2^{c0,a0}_{a3,a4} V^{a5,a6}_{a1,a2} eta1^{a4}_{a6} eta1^{a3}_{a5}
    into the einsum code string "H_caaa += +0.25000000 * np.einsum('iuvw,xyzr,wr,vz->iuxy', T2['caaa'], V['aaaa'], eta1, eta1, optimize='optimal')"
    """
    def _get_unique_tensor_indices(tensor, unused_indices, index_dict, bra_name=bra_name, ket_name=ket_name):
        indstr = ''
        if tensor.label() == bra_name:
            label_total = tensor.lower()+tensor.upper()
        else:
            label_total = tensor.upper()+tensor.lower()

        for i in label_total:
            i = str(i)
            if index_dict.get(i):
                indstr += index_dict[i]
            else:
                index = unused_indices[i[0]].pop(0)
                index_dict[i] = index
                indstr += index
        if tensor.label() == bra_name or tensor.label() == ket_name:
            return 'p' + indstr

        return indstr

    osi = wicked.osi()
    unused_indices = osi.to_dict()
    index_dict = {}

    lhs = equation.lhs()
    rhs = equation.rhs()
    factor = equation.rhs_factor()

    # holds the expression part of the einsum contraction, e.g., iuvw,xyzr,wr,vz->iuxy
    index_string = ''
    # holds the tensor label part of the einsum contraction, e.g., T2['caaa'], V['aaaa'], eta1, eta1, optimize='optimal')
    tensor_label_string = ''

    for t in rhs.tensors():
        if (t.label() == bra_name):
            lhs_tensor_label = "[\"" + ''.join([str(_)[0] for _ in t.lower()]) + ''.join([
                str(_)[0] for _ in t.upper()]) + "\"]"
            bra_index = _get_unique_tensor_indices(
                t, unused_indices, index_dict)
        else:
            tensor_label_string += t.label() + "[\"" + ''.join(
                [str(_)[0] for _ in t.upper()]) + ''.join([str(_)[0] for _ in t.lower()]) + "\"],"
            index_string += _get_unique_tensor_indices(
                t, unused_indices, index_dict)
            index_string += ','

    index_string = index_string[:-1] + f'->{bra_index}'
    tensor_label_string += "optimize='optimal')"

    # If the lhs is not a scalar
    if (lhs.tensors()[0].upper() != [] or lhs.tensors()[0].lower() != []):
        left = rhs.tensors()[0].label()
        left += ''.join([osi.label(_.space()) for _ in lhs.tensors()[0].upper()]) + \
            ''.join([osi.label(_.space()) for _ in lhs.tensors()[0].lower()])
        res_indx = _get_unique_tensor_indices(
            rhs.tensors(), unused_indices, index_dict)
        index_string += res_indx
    else:  # If it's scalar, just return the label.
        left = lhs.tensors()[0].label()

    einsum_string = left \
        + lhs_tensor_label \
        + ' ' \
        + f"+= {float(factor):+.8f} * np.einsum('"\
        + index_string \
        + "', " \
        + tensor_label_string

    return einsum_string


def generate_sigma_build(mbeq, matrix):
    if matrix == 'Hbar':
        code = [f"def build_sigma_vector_{matrix}(Hbar, gamma1, eta1, lambda2, lambda3, c):",
                "    sigma = {}",
                "    for key in c.keys():",
                "        sigma[key] = np.zeros(c[key].shape)"]
    else:
        code = [f"def build_sigma_vector_{matrix}(gamma1, eta1, lambda2, lambda3, c):",
                "    sigma = {}",
                "    for key in c.keys():",
                "        sigma[key] = np.zeros(c[key].shape)"]
    for eq in mbeq['|']:
        no_print = False
        rhs = eq.rhs()
        for t in rhs.tensors():
            if t.label() in ['lambda4', 'lambda5', 'lambda6']:
                no_print = True
                break
        if not no_print:
            code.append(
                f"    {compile_sigma_vector(eq, bra_name='bra', ket_name='c')}")

    code.append("    return sigma")
    funct = "\n".join(code)
    return funct
