import wicked

__all__ = ["string_to_expr"]


def string_to_expr(s):
    expr = wicked.Expression()
    for line in s.split("\n"):
        expr += wicked.expression(line)
    return expr
