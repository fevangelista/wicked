import wicked

__all__ = ["string_to_expr", "gen_op"]


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
        is_le_sorted = all(m[le[i]] <= m[le[i + 1]] for i in range(len(le) - 1))
        if is_le_sorted:
            for re in itertools.product(*a):
                is_re_sorted = all(m[re[i]] >= m[re[i + 1]] for i in range(len(re) - 1))
                if is_re_sorted:
                    if not diagonal:
                        if le != re:
                            terms.append(
                                " ".join([s + "+" for s in le]) + " " + " ".join(re)
                            )
                    else:
                        terms.append(
                            " ".join([s + "+" for s in le]) + " " + " ".join(re)
                        )
    return wicked.op(label, terms)
