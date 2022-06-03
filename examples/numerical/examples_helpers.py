import numpy as np


def generate_equation(mbeq, nocc, nvir):
    res_sym = f"R{'o' * nocc}{'v' * nvir}"
    code = [
        f"def evaluate_residual_{nocc}_{nvir}(H,T):",
        "    # contributions to the residual",
    ]

    if nocc + nvir == 0:
        code.append("    R = 0.0")
    else:
        dims = ",".join(["nocc"] * nocc + ["nvir"] * nvir)
        code.append(f"    {res_sym} = np.zeros(({dims}))")

    for eq in mbeq["o" * nocc + "|" + "v" * nvir]:
        contraction = eq.compile("einsum")
        code.append(f"    {contraction}")

    code.append(f"    return {res_sym}")
    funct = "\n".join(code)
    return funct


def antisymmetrize_residual_2_2(Roovv, nocc, nvir):
    # antisymmetrize the residual
    Roovv_anti = np.zeros((nocc, nocc, nvir, nvir))
    Roovv_anti += np.einsum("ijab->ijab", Roovv)
    Roovv_anti -= np.einsum("ijab->jiab", Roovv)
    Roovv_anti -= np.einsum("ijab->ijba", Roovv)
    Roovv_anti += np.einsum("ijab->jiba", Roovv)
    return Roovv_anti


def update_amplitudes_ccsd(T, R, invD):
    T["ov"] += np.einsum("ia,ia->ia", R["ov"], invD["ov"])
    T["oovv"] += np.einsum("ijab,ijab->ijab", R["oovv"], invD["oovv"])


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


def update_amplitudes_ccsdt(T, R, invD):
    T["ov"] += np.einsum("ia,ia->ia", R["ov"], invD["ov"])
    T["oovv"] += np.einsum("ijab,ijab->ijab", R["oovv"], invD["oovv"])
    T["ooovvv"] += np.einsum("ijkabc,ijkabc->ijkabc", R["ooovvv"], invD["ooovvv"])


def compute_inverse_denominators(H, nocc, nvir, rank):
    fo = np.diag(H["oo"])
    fv = np.diag(H["vv"])

    D = {}

    if rank < 1:
        return D

    d1 = np.zeros((nocc, nvir))
    for i in range(nocc):
        for a in range(nvir):
            si = i % 2
            sa = a % 2
            if si == sa:
                d1[i][a] = 1.0 / (fo[i] - fv[a])
    D["ov"] = d1

    if rank < 2:
        return D

    d2 = np.zeros((nocc, nocc, nvir, nvir))
    for i in range(nocc):
        for j in range(nocc):
            for a in range(nvir):
                for b in range(nvir):
                    si = i % 2
                    sj = j % 2
                    sa = a % 2
                    sb = b % 2
                    if si == sj == sa == sb:
                        d2[i][j][a][b] = 1.0 / (fo[i] + fo[j] - fv[a] - fv[b])
                    if si == sa and sj == sb and si != sj:
                        d2[i][j][a][b] = 1.0 / (fo[i] + fo[j] - fv[a] - fv[b])
                    if si == sb and sj == sa and si != sj:
                        d2[i][j][a][b] = 1.0 / (fo[i] + fo[j] - fv[a] - fv[b])
    D["oovv"] = d2

    if rank < 3:
        return D

    d3 = np.zeros((nocc, nocc, nocc, nvir, nvir, nvir))
    for i in range(nocc):
        for j in range(nocc):
            for k in range(nocc):
                for a in range(nvir):
                    for b in range(nvir):
                        for c in range(nvir):
                            si = i % 2
                            sj = j % 2
                            sk = k % 2
                            sa = a % 2
                            sb = b % 2
                            sc = c % 2
                            d3[i][j][k][a][b][c] = 1.0 / (
                                fo[i] + fo[j] + fo[k] - fv[a] - fv[b] - fv[c]
                            )
    D["ooovvv"] = d3
    return D
