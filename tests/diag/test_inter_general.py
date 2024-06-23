import wicked as w
import numpy as np

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

def test_inter_general_numerical():
    def gen_antisym_2(n):
        rand = np.random.rand(n,n,n,n)

        res = np.einsum('ijkl->ijkl', rand)
        res -= np.einsum('ijkl->jikl', rand)
        res -= np.einsum('ijkl->ijlk', rand)
        res += np.einsum('ijkl->jilk', rand)
        return res

    def gen_antisym_3(n):
        rand = np.random.rand(n,n,n,n,n,n)
        res = np.einsum('ijkabc->ijkabc', rand)
        res += +1 * np.einsum("ijkabc->ijkabc", rand)
        res += -1 * np.einsum("ijkabc->ijkacb", rand)
        res += -1 * np.einsum("ijkabc->ijkbac", rand)
        res += +1 * np.einsum("ijkabc->ijkbca", rand)
        res += +1 * np.einsum("ijkabc->ijkcab", rand)
        res += -1 * np.einsum("ijkabc->ijkcba", rand)
        res += -1 * np.einsum("ijkabc->ikjabc", rand)
        res += +1 * np.einsum("ijkabc->ikjacb", rand)
        res += +1 * np.einsum("ijkabc->ikjbac", rand)
        res += -1 * np.einsum("ijkabc->ikjbca", rand)
        res += -1 * np.einsum("ijkabc->ikjcab", rand)
        res += +1 * np.einsum("ijkabc->ikjcba", rand)
        res += -1 * np.einsum("ijkabc->jikabc", rand)
        res += +1 * np.einsum("ijkabc->jikacb", rand)
        res += +1 * np.einsum("ijkabc->jikbac", rand)
        res += -1 * np.einsum("ijkabc->jikbca", rand)
        res += -1 * np.einsum("ijkabc->jikcab", rand)
        res += +1 * np.einsum("ijkabc->jikcba", rand)
        res += +1 * np.einsum("ijkabc->jkiabc", rand)
        res += -1 * np.einsum("ijkabc->jkiacb", rand)
        res += -1 * np.einsum("ijkabc->jkibac", rand)
        res += +1 * np.einsum("ijkabc->jkibca", rand)
        res += +1 * np.einsum("ijkabc->jkicab", rand)
        res += -1 * np.einsum("ijkabc->jkicba", rand)
        res += +1 * np.einsum("ijkabc->kijabc", rand)
        res += -1 * np.einsum("ijkabc->kijacb", rand)
        res += -1 * np.einsum("ijkabc->kijbac", rand)
        res += +1 * np.einsum("ijkabc->kijbca", rand)
        res += +1 * np.einsum("ijkabc->kijcab", rand)
        res += -1 * np.einsum("ijkabc->kijcba", rand)
        res += -1 * np.einsum("ijkabc->kjiabc", rand)
        res += +1 * np.einsum("ijkabc->kjiacb", rand)
        res += +1 * np.einsum("ijkabc->kjibac", rand)
        res += -1 * np.einsum("ijkabc->kjibca", rand)
        res += -1 * np.einsum("ijkabc->kjicab", rand)
        res += +1 * np.einsum("ijkabc->kjicba", rand)
        return res

    def gen_antisym_3_aab(n):
        rand = np.random.rand(n,n,n,n,n,n)
        res = np.einsum('ijkabc->ijkabc', rand)
        res -= np.einsum("ijkabc->ijkbac", rand)
        res -= np.einsum("ijkabc->jikabc", rand)
        res += np.einsum("ijkabc->jikbac", rand)
        return res

    def gen_antisym_3_abb(n):
        rand = np.random.rand(n,n,n,n,n,n)
        res = np.einsum('ijkabc->ijkabc', rand)
        res -= np.einsum("ijkabc->ijkacb", rand)
        res -= np.einsum("ijkabc->ikjabc", rand)
        res += np.einsum("ijkabc->ikjacb", rand)
        return res
    w.reset_space()
    w.add_space('a', 'fermion', 'general', list('stuvwxyz'))
    w.add_space('A', 'fermion', 'general', list('STUVWXYZ'))
    wt = w.WickTheorem()

    Hop = w.op("H", ['A+ a+ A a', 'a+ a+ a a', 'A+ A+ A A'])
    Top = w.op("T", ['A+ A', 'a+ a'])
    val_si = wt.contract(Hop @ Top, 0, 0, inter_general=True)
    si_mbeq = val_si.to_manybody_equation('M')

    Hop = w.op("H", ['a+ a+ a a'])
    Top = w.op("T", ['a+ a'])
    val_so = wt.contract(Hop @ Top, 0, 0, inter_general=False)
    so_mbeq = val_so.to_manybody_equation('M')

    si_code = "def si_M(H,T,eta1,gamma1,lambda2,lambda3):\n   M = 0.0\n"
    so_code = "def so_M(H,T,eta1,gamma1,lambda2,lambda3):\n   M = 0.0\n"
    si_keys = dict()
    for i in si_mbeq['|']:
        si_code += '   ' + w.compile_einsum(i, si_keys) + '\n'

    for i in so_mbeq['|']:
        so_code += '   ' + w.compile_einsum(i) + '\n'

    si_code += '   return M\n'
    so_code += '   return M\n'
    exec(si_code, globals())
    exec(so_code, globals())

    n = 4
    H_si = {}
    T_si = {}
    eta_si = {}
    gamma_si = {}
    lambda2_si = {}
    lambda3_si = {}
    H_si['aaaa'] = gen_antisym_2(n)
    H_si['AAAA'] = gen_antisym_2(n)
    H_si['aAaA'] = np.random.rand(n,n,n,n)
    T_si['aa'] = np.random.rand(n,n)
    T_si['AA'] = np.random.rand(n,n)
    eta_si['aa'] = np.random.rand(n,n)
    eta_si['AA'] = np.random.rand(n,n)
    gamma_si['aa'] = np.random.rand(n,n)
    gamma_si['AA'] = np.random.rand(n,n)
    lambda2_si['aaaa'] = gen_antisym_2(n)
    lambda2_si['AAAA'] = gen_antisym_2(n)
    lambda2_si['aAaA'] = np.random.rand(n,n,n,n)
    lambda3_si['aaaaaa'] = gen_antisym_3(n)
    lambda3_si['AAAAAA'] = gen_antisym_3(n)
    lambda3_si['aaAaaA'] = gen_antisym_3_aab(n)
    lambda3_si['aAAaAA'] = gen_antisym_3_abb(n)

    H_so = {'aaaa': np.zeros((n*2,)*4)}
    T_so = {'aa': np.zeros((n*2,)*2)}
    eta_so = {'aa': np.zeros((n*2,)*2)}
    gamma_so = {'aa': np.zeros((n*2,)*2)}
    lambda2_so = {'aaaa': np.zeros((n*2,)*4)}
    lambda3_so = {'aaaaaa': np.zeros((n*2,)*6)}

    alpha = slice(0,2*n,2)
    beta = slice(1,2*n,2)
    sl = lambda x: alpha if x=='a' else beta
    for k in si_keys['H']:
        H_so['aaaa'][sl(k[0]), sl(k[1]), sl(k[2]), sl(k[3])] = H_si[k].copy()
    for k in si_keys['T']:
        T_so['aa'][sl(k[0]), sl(k[1])] = T_si[k].copy()
    for k in si_keys['eta1']:
        eta_so['aa'][sl(k[0]), sl(k[1])] = eta_si[k].copy()
    for k in si_keys['gamma1']:
        gamma_so['aa'][sl(k[0]), sl(k[1])] = gamma_si[k].copy()
    for k in si_keys['lambda2']:
        lambda2_so['aaaa'][sl(k[0]), sl(k[1]), sl(k[2]), sl(k[3])] = lambda2_si[k].copy()
    for k in si_keys['lambda3']:
        lambda3_so['aaaaaa'][sl(k[0]), sl(k[1]), sl(k[2]), sl(k[3]), sl(k[4]), sl(k[5])] = lambda3_si[k].copy()

    assert np.isclose(si_M(H_si, T_si, eta_si, gamma_si, lambda2_si, lambda3_si), so_M(H_so, T_so, eta_so, gamma_so, lambda2_so, lambda3_so))

if __name__ == "__main__":
    test_inter_general()
    test_inter_general_numerical()