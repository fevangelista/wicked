import numpy as np

def generate_equation(mbeq, nocc, nvir, space):
    space_flip = space.split('|')[0] + space.split('|')[1][::-1]
    res_sym = 'R' + space_flip
    code = [
        f"def evaluate_residual_{space_flip}(H,T):",
        "    # contributions to the residual",
    ]

    if nocc + nvir == 0:
        code.append("    R = 0.0")
    else:
        dims = ",".join(["nocc"] * nocc + ["nvir"] * nvir)
        code.append(f"    {res_sym} = np.zeros(({dims}))")

    for eq in mbeq[space]:
        contraction = eq.compile("einsum")
        code.append(f"    {contraction}")
    code.append(f"    return {res_sym}")
    funct = "\n".join(code)
    return funct

def update_cc_amplitudes(T, R, invD, rank: int):
    """
    A function to updated the CCSD amplitudes

    Parameters
    ----------
    T : dict[np.ndarray]
        The cluster amplitudes
    R : dict[np.ndarray]
        The CC residual
    invD : dict[np.ndarray]
        The inverse MP denominators
    rank : int
        The rank of the CC equations (e.g., CCSD : rank = 2)
    """
    if rank >= 1:
        T["ov"] += np.einsum("ia,ia->ia", R["ov"], invD["ov"])
        T["OV"] += np.einsum("IA,IA->IA", R["OV"], invD["OV"])
    if rank >= 2:
        T["oovv"] += np.einsum("ijab,ijab->ijab", R["oovv"], invD["oovv"])
        T["OOVV"] += np.einsum("IJAB,IJAB->IJAB", R["OOVV"], invD["OOVV"])
        T["oOvV"] += np.einsum("iJaB,iJaB->iJaB", R["oOvV"], invD["oOvV"])

def antisymmetrize_residual_2_2(Roovv, nocc, nvir):
    # antisymmetrize the residual
    Roovv_anti = np.zeros((nocc, nocc, nvir, nvir))
    Roovv_anti += np.einsum("ijab->ijab", Roovv)
    Roovv_anti -= np.einsum("ijab->jiab", Roovv)
    Roovv_anti -= np.einsum("ijab->ijba", Roovv)
    Roovv_anti += np.einsum("ijab->jiba", Roovv)
    return Roovv_anti

def compute_inverse_denominators(H: dict, nocc: list[int], nvir: list[int], rank: int):
    """
    A function to compute the inverse of Møller-Plesset denominators
    """
    fo = np.diag(H["oo"])
    fv = np.diag(H["vv"])

    D = {}

    if rank >= 1:
        D["ov"] = 1.0 / (fo.reshape(-1, 1) - fv)
        D["OV"] = D["ov"]

    if rank >= 2:
        D["oovv"] = 1.0 / (
            fo.reshape(-1, 1, 1, 1) + fo.reshape(-1, 1, 1) - fv.reshape(-1, 1) - fv
        )
        D["OOVV"] = D["oovv"]
        D["oOvV"] = D["oovv"]
    if rank > 2:
        raise ValueError(
            f"compute_inverse_denominators() supports rank up to 2, but was called with rank = {rank}"
        )
    return D
