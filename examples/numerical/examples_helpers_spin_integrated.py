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
    A function to update the CCSD amplitudes

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
    if rank >= 3:
        T["ooovvv"] += np.einsum("ijkabc,ijkabc->ijkabc", R["ooovvv"], invD["ooovvv"])
        T["OOOVVV"] += np.einsum("IJKABC,IJKABC->IJKABC", R["OOOVVV"], invD["OOOVVV"])
        T["ooOvvV"] += np.einsum("ijKabC,ijKabC->ijKabC", R["ooOvvV"], invD["ooOvvV"])
        T["oOOvVV"] += np.einsum("iJKaBC,iJKaBC->iJKaBC", R["oOOvVV"], invD["oOOvVV"])

def antisymmetrize_residual_2_2(Roovv, nocc, nvir):
    # antisymmetrize the residual
    Roovv_anti = np.zeros((nocc, nocc, nvir, nvir))
    Roovv_anti += np.einsum("ijab->ijab", Roovv)
    Roovv_anti -= np.einsum("ijab->jiab", Roovv)
    Roovv_anti -= np.einsum("ijab->ijba", Roovv)
    Roovv_anti += np.einsum("ijab->jiba", Roovv)
    return Roovv_anti

def antisymmetrize_residual_3_3(Rooovvv, nocc, nvir):
    # antisymmetrize the residual
    Rooovvv_anti = np.zeros((nocc, nocc, nocc, nvir, nvir, nvir))
    Rooovvv_anti += +1 * np.einsum("ijkabc->ijkabc", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->ijkacb", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->ijkbac", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->ijkbca", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->ijkcab", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->ijkcba", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->ikjabc", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->ikjacb", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->ikjbac", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->ikjbca", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->ikjcab", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->ikjcba", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->jikabc", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->jikacb", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->jikbac", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->jikbca", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->jikcab", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->jikcba", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->jkiabc", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->jkiacb", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->jkibac", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->jkibca", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->jkicab", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->jkicba", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->kijabc", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->kijacb", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->kijbac", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->kijbca", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->kijcab", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->kijcba", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->kjiabc", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->kjiacb", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->kjibac", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->kjibca", Rooovvv)
    Rooovvv_anti += -1 * np.einsum("ijkabc->kjicab", Rooovvv)
    Rooovvv_anti += +1 * np.einsum("ijkabc->kjicba", Rooovvv)
    return Rooovvv_anti

def antisymmetrize_residual_3_3_aab(RooOvvV, nocc, nvir):
    # antisymmetrize the residual
    RooOvvV_anti = np.zeros((nocc, nocc, nocc, nvir, nvir, nvir))
    RooOvvV_anti += np.einsum("ijkabc->ijkabc", RooOvvV)
    RooOvvV_anti -= np.einsum("ijkabc->ijkbac", RooOvvV)
    RooOvvV_anti -= np.einsum("ijkabc->jikabc", RooOvvV)
    RooOvvV_anti += np.einsum("ijkabc->jikbac", RooOvvV)
    return RooOvvV_anti

def antisymmetrize_residual_3_3_abb(RoOOvVV, nocc, nvir):
    # antisymmetrize the residual
    RoOOvVV_anti = np.zeros((nocc, nocc, nocc, nvir, nvir, nvir))
    RoOOvVV_anti += np.einsum("ijkabc->ijkabc", RoOOvVV)
    RoOOvVV_anti -= np.einsum("ijkabc->ijkacb", RoOOvVV)
    RoOOvVV_anti -= np.einsum("ijkabc->ikjabc", RoOOvVV)
    RoOOvVV_anti += np.einsum("ijkabc->ikjacb", RoOOvVV)
    return RoOOvVV_anti

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
    if rank >= 3:
        D["ooovvv"] = 1.0 / (
            fo.reshape(-1, 1, 1, 1, 1, 1)
            + fo.reshape(-1, 1, 1, 1, 1)
            + fo.reshape(-1, 1, 1, 1)
            - fv.reshape(-1, 1, 1)
            - fv.reshape(-1, 1)
            - fv
        )
        D["OOOVVV"] = D["ooovvv"]
        D["ooOvvV"] = D["ooovvv"]
        D["oOOvVV"] = D["ooovvv"]

    if rank > 3:
        raise ValueError(
            f"compute_inverse_denominators() supports rank up to 3, but was called with rank = {rank}"
        )
    return D

def get_index_blocks(func):
    """
    A function to gather the necessary blocks of the Hamiltonian for the CC equations

    Parameters
    ----------
    funt : str
        The function(s) to parse.
    """
    blocks = set()
    for line in func.split("\n"):
        if "np.einsum" in line and "H[" in line: # is a contraction containing H
            blocks.add(line.split("H[\"")[1].split("\"]")[0])

    return blocks
