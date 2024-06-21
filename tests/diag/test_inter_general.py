import wicked as w
def print_comparison(val, val2):
    print(f"\n{'-'*30} Result {'-'*30}\n{val}")
    print(f"{'-'*31} Test {'-'*31}\n{val2}")

def test_inter_general():
    w.reset_space()
    w.add_space('a', 'fermion', 'general', list('uvwxyz'))
    w.add_space('A', 'fermion', 'general', list('UVWXYZ'))
    wt = w.WickTheorem()

    Hop = w.op("H", ['A+ A'])
    Top = w.op("T", ['a+ a'])

    val = wt.contract(Hop @ Top, 0, 0, inter_general=True)

    ref = w.utils.string_to_expr("H^{A1}_{A0} T^{a1}_{a0} lambda2^{a0,A0}_{a1,A1}")
    print_comparison(val, ref)
    assert val == ref

if __name__ == "__main__":
    test_inter_general()