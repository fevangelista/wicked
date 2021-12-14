import wicked as w


def test_combinatorics():
    assert w.integer_partitions(2, 2) == [[1, 1], [2]]
    assert w.integer_partitions(2, 1) == [[2]]
    assert w.integer_partitions(3, 3) == [[1, 1, 1], [2, 1], [3]]
    assert w.integer_partitions(3, 2) == [[2, 1], [3]]
    assert w.integer_partitions(3, 1) == [[3]]


if __name__ == "__main__":
    test_combinatorics()
