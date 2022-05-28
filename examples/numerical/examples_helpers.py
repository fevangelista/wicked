import numpy as np

def antisymmetrize_residual_2_2(Roovv,nocc,nvir):
    # antisymmetrize the residual
    Roovv_anti = np.zeros((nocc,nocc,nvir,nvir))
    Roovv_anti += np.einsum("ijab->ijab",Roovv)
    Roovv_anti -= np.einsum("ijab->jiab",Roovv)
    Roovv_anti -= np.einsum("ijab->ijba",Roovv)
    Roovv_anti += np.einsum("ijab->jiba",Roovv)    
    return Roovv_anti

def update_amplitudes_ccsd(T,R,invD):
    T['ov'] += np.einsum("ia,ia->ia",R['ov'],invD['ov'])
    T['oovv'] += np.einsum("ijab,ijab->ijab",R['oovv'],invD['oovv'])
    
def generate_equation(mbeq, nocc, nvir):
    res_sym = f"R{'o' * nocc}{'v' * nvir}"
    code = [f"def evaluate_residual_{nocc}_{nvir}(H,T):",
        "    # contributions to the residual"]
    
    if nocc + nvir == 0:
        code.append("    R = 0.0")
    else:
        dims = ','.join(['nocc'] * nocc + ['nvir'] * nvir)
        code.append(f"    {res_sym} = np.zeros(({dims}))")
        
    for eq in mbeq["o" * nocc + "|" + "v" * nvir]:
        contraction = eq.compile("einsum")
        code.append(f'    {contraction}')
        
    code.append(f'    return {res_sym}')
    funct = '\n'.join(code)
    return funct    