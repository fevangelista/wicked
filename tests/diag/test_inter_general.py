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
        rand += rand.transpose(2,3,0,1)
        res = np.einsum('ijkl->ijkl', rand)
        res -= np.einsum('ijkl->jikl', rand)
        res -= np.einsum('ijkl->ijlk', rand)
        res += np.einsum('ijkl->jilk', rand)
        return res

    def gen_antisym_3(n):
        rand = np.random.rand(n,n,n,n,n,n)
        rand += rand.transpose(3,4,5,0,1,2)
        res = np.einsum('ijkabc->ijkabc', rand)
        res += -1 * np.einsum("ijkabc->ijkcab", rand)
        res += -1 * np.einsum("ijkabc->ijkbca", rand)
        res += -1 * np.einsum('ijkabc->kijabc', rand)
        res += +1 * np.einsum("ijkabc->kijcab", rand)
        res += -1 * np.einsum("ijkabc->kijbca", rand)
        res += -1 * np.einsum('ijkabc->jkiabc', rand)
        res += -1 * np.einsum("ijkabc->jkicab", rand)
        res += +1 * np.einsum("ijkabc->jkibca", rand)
        return res

    def gen_antisym_3_aab(n):
        rand = np.random.rand(n,n,n,n,n,n)
        rand += rand.transpose(3,4,5,0,1,2)
        res = np.einsum('ijkabc->ijkabc', rand)
        res -= np.einsum("ijkabc->ijkbac", rand)
        res -= np.einsum("ijkabc->jikabc", rand)
        res += np.einsum("ijkabc->jikbac", rand)
        return res

    def gen_antisym_3_abb(n):
        rand = np.random.rand(n,n,n,n,n,n)
        rand += rand.transpose(3,4,5,0,1,2)
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
    for idx,i in enumerate(si_mbeq['|']):
        si_code += '   ' + w.compile_einsum(i, si_keys) + '\n'

    for idx,i in enumerate(so_mbeq['|']):
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
    H_si['aAaA'] = gen_antisym_2(n)
    T_si['aa'] = np.random.rand(n,n)
    T_si['aa'] += T_si['aa'].T
    T_si['AA'] = np.random.rand(n,n)
    T_si['AA'] += T_si['AA'].T
    eta_si['aa'] = np.random.rand(n,n)
    eta_si['aa'] += eta_si['aa'].T
    eta_si['AA'] = np.random.rand(n,n)
    eta_si['AA'] += eta_si['AA'].T
    gamma_si['aa'] = np.random.rand(n,n)
    gamma_si['aa'] += gamma_si['aa'].T
    gamma_si['AA'] = np.random.rand(n,n)
    gamma_si['AA'] += gamma_si['AA'].T
    lambda2_si['aaaa'] = gen_antisym_2(n)
    lambda2_si['AAAA'] = gen_antisym_2(n)
    lambda2_si['aAaA'] = gen_antisym_2(n)
    lambda3_si['aaaaaa'] = gen_antisym_3(n)
    lambda3_si['AAAAAA'] = gen_antisym_3(n)
    lambda3_si['aaAaaA'] = gen_antisym_3(n)
    lambda3_si['aAAaAA'] = gen_antisym_3(n)

    H_so = {'aaaa': np.zeros((n*2,)*4)}
    T_so = {'aa': np.zeros((n*2,)*2)}
    eta_so = {'aa': np.zeros((n*2,)*2)}
    gamma_so = {'aa': np.zeros((n*2,)*2)}
    lambda2_so = {'aaaa': np.zeros((n*2,)*4)}
    lambda3_so = {'aaaaaa': np.zeros((n*2,)*6)}

    alpha = slice(0,2*n,2)
    beta = slice(1,2*n,2)

    H_so['aaaa'][alpha,alpha,alpha,alpha] = H_si['aaaa'].copy()
    H_so['aaaa'][beta,beta,beta,beta] = H_si['AAAA'].copy()
    H_so['aaaa'][alpha,beta,alpha,beta] = H_si['aAaA'].copy()
    H_so['aaaa'][beta,alpha,alpha,beta] = -H_si['aAaA'].copy()
    H_so['aaaa'][alpha,beta,beta,alpha] = -H_si['aAaA'].copy()
    H_so['aaaa'][beta,alpha,beta,alpha] = H_si['aAaA'].copy()

    T_so['aa'][alpha,alpha] = T_si['aa'].copy()
    T_so['aa'][beta,beta] = T_si['AA'].copy()

    eta_so['aa'][alpha,alpha] = eta_si['aa'].copy()
    eta_so['aa'][beta,beta] = eta_si['AA'].copy()

    gamma_so['aa'][alpha,alpha] = gamma_si['aa'].copy()
    gamma_so['aa'][beta,beta] = gamma_si['AA'].copy()

    lambda2_so['aaaa'][alpha,alpha,alpha,alpha] = lambda2_si['aaaa'].copy()
    lambda2_so['aaaa'][beta,beta,beta,beta] = lambda2_si['AAAA'].copy()
    lambda2_so['aaaa'][alpha,beta,alpha,beta] = lambda2_si['aAaA'].copy()
    lambda2_so['aaaa'][beta,alpha,alpha,beta] = -lambda2_si['aAaA'].copy()
    lambda2_so['aaaa'][alpha,beta,beta,alpha] = -lambda2_si['aAaA'].copy()
    lambda2_so['aaaa'][beta,alpha,beta,alpha] = lambda2_si['aAaA'].copy()

    lambda3_so['aaaaaa'][alpha, alpha, alpha, alpha, alpha, alpha] = lambda3_si['aaaaaa'].copy()
    lambda3_so['aaaaaa'][beta, beta, beta, beta, beta, beta] = lambda3_si['AAAAAA'].copy()

    lambda3_so['aaaaaa'][alpha, alpha, beta, alpha, alpha, beta] = lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][alpha, alpha, beta, alpha, beta, alpha] = -lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][alpha, alpha, beta, beta, alpha, alpha] = -lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][alpha, beta, alpha, alpha, alpha, beta] = -lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][alpha, beta, alpha, alpha, beta, alpha] = lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][alpha, beta, alpha, beta, alpha, alpha] = -lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][beta, alpha, alpha, alpha, alpha, beta] = -lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][beta, alpha, alpha, alpha, beta, alpha] = -lambda3_si['aaAaaA'].copy()
    lambda3_so['aaaaaa'][beta, alpha, alpha, beta, alpha, alpha] = lambda3_si['aaAaaA'].copy()

    lambda3_so['aaaaaa'][alpha, beta, beta, alpha, beta, beta] = lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][alpha, beta, beta, beta, alpha, beta] = -lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][alpha, beta, beta, beta, beta, alpha] = -lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][beta, alpha, beta, alpha, beta, beta] = -lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][beta, alpha, beta, beta, alpha, beta] = lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][beta, alpha, beta, beta, beta, alpha] = -lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][beta, beta, alpha, alpha, beta, beta] = -lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][beta, beta, alpha, beta, alpha, beta] = -lambda3_si['aAAaAA'].copy()
    lambda3_so['aaaaaa'][beta, beta, alpha, beta, beta, alpha] = lambda3_si['aAAaAA'].copy()

    print(f'si_M = {si_M(H_si, T_si, eta_si, gamma_si, lambda2_si, lambda3_si)}')
    print(f'so_M = {so_M(H_so, T_so, eta_so, gamma_so, lambda2_so, lambda3_so)}')
    print(f'Ratio = {si_M(H_si, T_si, eta_si, gamma_si, lambda2_si, lambda3_si)/so_M(H_so, T_so, eta_so, gamma_so, lambda2_so, lambda3_so):.10f}')
    assert np.isclose(si_M(H_si, T_si, eta_si, gamma_si, lambda2_si, lambda3_si)/so_M(H_so, T_so, eta_so, gamma_so, lambda2_so, lambda3_so),1.0)

if __name__ == "__main__":
    test_inter_general()
    test_inter_general_numerical()