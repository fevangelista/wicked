import wicked as w


def test_tensor():
    """Test the Tensor class"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j"])
    w.add_space("a", "fermion", "general", ["u", "v"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c"])

    # Create a tensor from a string
    t = w.tensor("T^{v0}_{a0}", w.sym.none)
    assert str(t) == "T^{v0}_{a0}"

    # Create a tensor from a string
    t = w.tensor("T^{o_0,o_1}_{v_0,v_1}", w.sym.none)
    assert str(t) == "T^{o0,o1}_{v0,v1}"

    # Create a tensor with one lower index
    t = w.tensor("T", ["o_0"], [], w.sym.none)
    assert str(t) == "T^{}_{o0}"

    # Create a tensor with one upper index
    t = w.tensor("T", [], ["o_0"], w.sym.none)
    assert str(t) == "T^{o0}_{}"

    # Create a tensor with lower and upper indices
    t = w.tensor("T", ["v_0"], ["o_0"], w.sym.none)
    assert str(t) == "T^{o0}_{v0}"

    # Create a tensor with lower and upper indices
    t = w.tensor("T", ["v_0", "a_0"], ["o_0", "a_1"], w.sym.none)
    assert str(t) == "T^{o0,a1}_{v0,a0}"
    assert t.latex() == "{T}^{i v}_{a u}"

    t = w.tensor("T", ["v_0", "v_1"], ["a_0", "a_1"], w.sym.none)
    assert t.symmetry_factor() == 4


if __name__ == "__main__":
    test_tensor()
