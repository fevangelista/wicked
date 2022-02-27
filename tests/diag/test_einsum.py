import wicked as w


def print_comparison(val, val2):
    print(f"Result: {val}")
    print(f"Test:   {val2}")


def initialize():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])


def test_energy():
    """CCSD energy"""
    initialize()
    T1 = w.op("T1", ["v+ o"])
    T2 = w.op("T2", ["v+ v+ o o"])
    F = w.op("f", ["o+ o", "o+ v", "v+ o", "v+ v"])
    V = w.op(
        "v",
        [
            "o+ o+ o o",
            "o+ o+ v o",
            "o+ o+ v v",
            "o+ v+ o o",
            "o+ v+ v o",
            "o+ v+ v v",
            "v+ v+ o o",
            "v+ v+ v o",
            "v+ v+ v v",
        ],
    )

    wt = w.WickTheorem()

    expr = wt.contract(w.rational(1), F @ T1, 0, 2)
    mbeq = expr.to_manybody_equation("R")

    assert len(mbeq["|"]) == 1
    comp = mbeq["|"][0].compile("einsum")
    print(comp)
    assert comp == 'R += 1.000000000 * np.einsum("ia,ai->",T1["ov"],f["vo"])'

    assert len(mbeq["o|v"]) == 2
    comp = mbeq["o|v"][1].compile("einsum")
    print(comp)
    assert comp == 'Rov += 1.000000000 * np.einsum("ib,ba->ia",T1["ov"],f["vv"])'

    comp = mbeq["o|v"][0].compile("einsum")
    print(comp)
    assert comp == 'Rov += -1.000000000 * np.einsum("ja,ij->ia",T1["ov"],f["oo"])'

    assert len(mbeq["o|o"]) == 1
    comp = mbeq["o|o"][0].compile("einsum")
    print(comp)
    assert comp == 'Roo += 1.000000000 * np.einsum("ia,aj->ij",T1["ov"],f["vo"])'

    assert len(mbeq["v|v"]) == 1
    comp = mbeq["v|v"][0].compile("einsum")
    print(comp)
    assert comp == 'Rvv += -1.000000000 * np.einsum("ia,bi->ba",T1["ov"],f["vo"])'

    expr = wt.contract(w.rational(1), V @ T2, 0, 2)
    mbeq = expr.to_manybody_equation("R")

    assert len(mbeq["|"]) == 1
    comp = mbeq["|"][0].compile("einsum")
    print(comp)
    assert comp == 'R += 0.250000000 * np.einsum("ijab,abij->",T2["oovv"],v["vvoo"])'


if __name__ == "__main__":
    test_energy()
