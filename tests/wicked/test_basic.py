def test_import():
    import wicked


def test_orbital_space():
    import wicked

    wicked.reset_space()
    assert wicked.num_spaces() == 0


if __name__ == "__main__":
    test_import()
    test_orbital_space()
