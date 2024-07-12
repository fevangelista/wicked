import wicked as w

def test_gen_op():
    w.reset_space()
    w.add_space('c', 'fermion', 'occupied', list('klmn'))
    w.add_space('v', 'fermion', 'unoccupied', list('efgh'))
    w.add_space('a', 'fermion', 'general', list('oabrstuvwxyz'))

    w.add_space('C', 'fermion', 'occupied', list('KLMN'))
    w.add_space('V', 'fermion', 'unoccupied', list('EFGH'))
    w.add_space('A', 'fermion', 'general', list('OABRSTUVWXYZ'))
    F = w.gen_op('F', 1, 'cav', 'cav', diagonal=False)
    assert str(F) == \
    '+ F { a+ v }\n+ F { v+ a }\n+ F { a+ c }\n+ F { v+ c }\n+ F { c+ a }\n+ F { c+ v }'
    
    V = w.gen_op('V', 2, 'cav', 'cav', diagonal=True)
    assert str(V) == \
    '+ 1/4 V { a+ a+ a a }\n+ 1/2 V { a+ a+ a v }\n+ 1/4 V { a+ a+ v v }\n+ 1/2 V { v+ a+ a a }\n+ V { v+ a+ a v }\n+ 1/2 V { v+ a+ v v }\n+ 1/4 V { v+ v+ a a }\n+ 1/2 V { v+ v+ a v }\n+ 1/4 V { v+ v+ v v }\n+ 1/2 V { a+ a+ a c }\n+ 1/2 V { a+ a+ v c }\n+ V { v+ a+ a c }\n+ V { v+ a+ v c }\n+ 1/2 V { v+ v+ a c }\n+ 1/2 V { v+ v+ v c }\n+ 1/4 V { a+ a+ c c }\n+ 1/2 V { v+ a+ c c }\n+ 1/4 V { v+ v+ c c }\n+ 1/2 V { c+ a+ a a }\n+ V { c+ a+ a v }\n+ 1/2 V { c+ a+ v v }\n+ 1/2 V { c+ v+ a a }\n+ V { c+ v+ a v }\n+ 1/2 V { c+ v+ v v }\n+ V { c+ a+ a c }\n+ V { c+ a+ v c }\n+ V { c+ v+ a c }\n+ V { c+ v+ v c }\n+ 1/2 V { c+ a+ c c }\n+ 1/2 V { c+ v+ c c }\n+ 1/4 V { c+ c+ a a }\n+ 1/2 V { c+ c+ a v }\n+ 1/4 V { c+ c+ v v }\n+ 1/2 V { c+ c+ a c }\n+ 1/2 V { c+ c+ v c }\n+ 1/4 V { c+ c+ c c }'

def test_gen_op_ms0():
    w.reset_space()
    w.add_space('c', 'fermion', 'occupied', list('klmn'))
    w.add_space('v', 'fermion', 'unoccupied', list('efgh'))
    w.add_space('a', 'fermion', 'general', list('oabrstuvwxyz'))

    w.add_space('C', 'fermion', 'occupied', list('KLMN'))
    w.add_space('V', 'fermion', 'unoccupied', list('EFGH'))
    w.add_space('A', 'fermion', 'general', list('OABRSTUVWXYZ'))
    F = w.gen_op_ms0('F', 1, 'cav', 'cav', diagonal=False)
    assert str(F) == \
    '+ F { A+ V }\n+ F { V+ A }\n+ F { A+ C }\n+ F { V+ C }\n+ F { C+ A }\n+ F { C+ V }\n+ F { a+ v }\n+ F { v+ a }\n+ F { a+ c }\n+ F { v+ c }\n+ F { c+ a }\n+ F { c+ v }'

    V = w.gen_op_ms0('V', 2, 'cav', 'cav', diagonal=False)
    assert str(V) == \
    '+ 1/2 V { A+ A+ A V }\n+ 1/4 V { A+ A+ V V }\n+ 1/2 V { V+ A+ A A }\n+ V { V+ A+ A V }\n+ 1/2 V { V+ A+ V V }\n+ 1/4 V { V+ V+ A A }\n+ 1/2 V { V+ V+ A V }\n+ 1/2 V { A+ A+ A C }\n+ 1/2 V { A+ A+ V C }\n+ V { V+ A+ A C }\n+ V { V+ A+ V C }\n+ 1/2 V { V+ V+ A C }\n+ 1/2 V { V+ V+ V C }\n+ 1/4 V { A+ A+ C C }\n+ 1/2 V { V+ A+ C C }\n+ 1/4 V { V+ V+ C C }\n+ 1/2 V { C+ A+ A A }\n+ V { C+ A+ A V }\n+ 1/2 V { C+ A+ V V }\n+ 1/2 V { C+ V+ A A }\n+ V { C+ V+ A V }\n+ 1/2 V { C+ V+ V V }\n+ V { C+ A+ A C }\n+ V { C+ A+ V C }\n+ V { C+ V+ A C }\n+ V { C+ V+ V C }\n+ 1/2 V { C+ A+ C C }\n+ 1/2 V { C+ V+ C C }\n+ 1/4 V { C+ C+ A A }\n+ 1/2 V { C+ C+ A V }\n+ 1/4 V { C+ C+ V V }\n+ 1/2 V { C+ C+ A C }\n+ 1/2 V { C+ C+ V C }\n+ V { a+ A+ V a }\n+ V { a+ V+ A a }\n+ V { a+ V+ V a }\n+ V { a+ A+ C a }\n+ V { a+ V+ C a }\n+ V { a+ C+ A a }\n+ V { a+ C+ V a }\n+ V { a+ C+ C a }\n+ V { a+ A+ A v }\n+ V { a+ A+ V v }\n+ V { a+ V+ A v }\n+ V { a+ V+ V v }\n+ V { a+ A+ C v }\n+ V { a+ V+ C v }\n+ V { a+ C+ A v }\n+ V { a+ C+ V v }\n+ V { a+ C+ C v }\n+ 1/2 V { a+ a+ a v }\n+ 1/4 V { a+ a+ v v }\n+ V { v+ A+ A a }\n+ V { v+ A+ V a }\n+ V { v+ V+ A a }\n+ V { v+ V+ V a }\n+ V { v+ A+ C a }\n+ V { v+ V+ C a }\n+ V { v+ C+ A a }\n+ V { v+ C+ V a }\n+ V { v+ C+ C a }\n+ 1/2 V { v+ a+ a a }\n+ V { v+ A+ A v }\n+ V { v+ A+ V v }\n+ V { v+ V+ A v }\n+ V { v+ A+ C v }\n+ V { v+ V+ C v }\n+ V { v+ C+ A v }\n+ V { v+ C+ V v }\n+ V { v+ C+ C v }\n+ V { v+ a+ a v }\n+ 1/2 V { v+ a+ v v }\n+ 1/4 V { v+ v+ a a }\n+ 1/2 V { v+ v+ a v }\n+ V { a+ A+ A c }\n+ V { a+ A+ V c }\n+ V { a+ V+ A c }\n+ V { a+ V+ V c }\n+ V { a+ A+ C c }\n+ V { a+ V+ C c }\n+ V { a+ C+ A c }\n+ V { a+ C+ V c }\n+ V { a+ C+ C c }\n+ 1/2 V { a+ a+ a c }\n+ 1/2 V { a+ a+ v c }\n+ V { v+ A+ A c }\n+ V { v+ A+ V c }\n+ V { v+ V+ A c }\n+ V { v+ V+ V c }\n+ V { v+ A+ C c }\n+ V { v+ V+ C c }\n+ V { v+ C+ A c }\n+ V { v+ C+ V c }\n+ V { v+ C+ C c }\n+ V { v+ a+ a c }\n+ V { v+ a+ v c }\n+ 1/2 V { v+ v+ a c }\n+ 1/2 V { v+ v+ v c }\n+ 1/4 V { a+ a+ c c }\n+ 1/2 V { v+ a+ c c }\n+ 1/4 V { v+ v+ c c }\n+ V { c+ A+ A a }\n+ V { c+ A+ V a }\n+ V { c+ V+ A a }\n+ V { c+ V+ V a }\n+ V { c+ A+ C a }\n+ V { c+ V+ C a }\n+ V { c+ C+ A a }\n+ V { c+ C+ V a }\n+ V { c+ C+ C a }\n+ 1/2 V { c+ a+ a a }\n+ V { c+ A+ A v }\n+ V { c+ A+ V v }\n+ V { c+ V+ A v }\n+ V { c+ V+ V v }\n+ V { c+ A+ C v }\n+ V { c+ V+ C v }\n+ V { c+ C+ A v }\n+ V { c+ C+ V v }\n+ V { c+ C+ C v }\n+ V { c+ a+ a v }\n+ 1/2 V { c+ a+ v v }\n+ 1/2 V { c+ v+ a a }\n+ V { c+ v+ a v }\n+ 1/2 V { c+ v+ v v }\n+ V { c+ A+ A c }\n+ V { c+ A+ V c }\n+ V { c+ V+ A c }\n+ V { c+ V+ V c }\n+ V { c+ A+ C c }\n+ V { c+ V+ C c }\n+ V { c+ C+ A c }\n+ V { c+ C+ V c }\n+ V { c+ a+ a c }\n+ V { c+ a+ v c }\n+ V { c+ v+ a c }\n+ V { c+ v+ v c }\n+ 1/2 V { c+ a+ c c }\n+ 1/2 V { c+ v+ c c }\n+ 1/4 V { c+ c+ a a }\n+ 1/2 V { c+ c+ a v }\n+ 1/4 V { c+ c+ v v }\n+ 1/2 V { c+ c+ a c }\n+ 1/2 V { c+ c+ v c }'

def test_dict_to_einsum():
    d = {'factor':0.25,'lhs':[['H','','xyvw']],'rhs':[['T2','caaa','iuvw'],['V','aaca','xyiz'],['gamma1','aa','zu']]}
    eq = w.dict_to_einsum(d)
    assert eq == "H += +0.25000000 * np.einsum('iuvw,xyiz,zu->xyvw',T2['caaa'],V['aaca'],gamma1['aa'],optimize='optimal')"

def test_compile_einsum():
    w.reset_space()
    w.add_space("c", "fermion", "occupied", list('ijklmn'))
    w.add_space("a", "fermion", "general", list('uvwxyzrst'))
    w.add_space("v", "fermion", "unoccupied", list('abcdef'))
    wt = w.WickTheorem()

    F = w.utils.gen_op('F',1,'cav','cav',diagonal=True)
    T2op = w.utils.gen_op('T2',2,'av','ca',diagonal=False)
    FT2 = wt.contract(w.commutator(F, T2op), 2, 2).to_manybody_equation("H")['c|v']
    eq = w.compile_einsum(FT2[-1])
    assert eq == """H['cv'] += +1.00000000 * np.einsum('au,ivba,uv->ib', F["va"],T2["cavv"],gamma1["aa"],optimize='optimal')"""

if __name__ == "__main__":
    test_gen_op()
    test_gen_op_ms0()
    test_dict_to_einsum()
    test_compile_einsum()