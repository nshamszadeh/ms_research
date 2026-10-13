# Neuron-Dropout Faults in Fractional-Order Reservoirs: A Theoretical Analysis

Oct 4, 2026 · @Spacebar

Clamping a neuron's weights to zero acts on a fractional-order reservoir (FOR) as an injected "anti-signal": the exact negative of what that neuron used to send. The reservoir's response is that forcing passed through the fractional dynamics. In a linearized model this yields closed-form results with five consequences:

- **The path depends on α.** After a permanent fault the reservoir relaxes to its new state as a power law, t^(−α), not exponentially. With an untruncated Grünwald–Letnikov (GL) history there is no characteristic recovery time.
- **The endpoint does not.** The fault's shift of the mean operating point is independent of α. With a truncated GL buffer, α and L enter only through an effective leak.
- **In hardware, the buffer length matters as much as α.** Truncation at L steps caps recovery at about 1.5L to 2.5L steps. For short buffers, recovery time peaks at intermediate α.
- **Lost memory capacity with a retrained readout is exactly 1 per neuron in the linear, noise-free limit.** That holds for every α. Any α-dependence of fault tolerance measured with retraining therefore comes from noise, spiking nonlinearity or quantization, which tells you where to look.
- **Random dropout moves the reservoir toward the ordered regime.** The effective coupling gain shrinks by a factor of √(1 − p) when a fraction p of neurons is removed.

The derivations below use the GL-FLIF model and the information measures of Mastin et al. (2026). A numerical check of the main scaling laws is included.

## Model and fault definitions

The analysis linearizes the spiking GL-FLIF reservoir of Mastin et al. into a linear fractional-order network. That model is exactly solvable, and its predictions are then checked against the spiking model.

### The GL-FLIF reservoir

Each neuron i follows Algorithm 1 of Mastin et al., with Grünwald–Letnikov coefficients c\_k = (−1)^k binom(α, k) and a history buffer of length L:

```latex
V_{i,n} = \Delta t^{\alpha}\left( -\frac{V_{i,n-1} - V_{rest}}{\tau_m} + I_{i,n} + b \right) - \sum_{k=1}^{L} c_k \, V_{i,n-k}, \qquad I_{i,n} = \sum_j W_{ij}\, s_{j,n-1} + \sum_m W^{in}_{im}\, u_{m,n}
```

with threshold V\_th, reset to V\_reset, spikes s\_j and a linear readout y\_n = W\_out x\_n. For 0 < α < 1, every c\_k with k ≥ 1 is negative and Σ\_{k≥0} c\_k = 0. So −Σ c\_k V\_{n−k} is a weighted average of past potentials whose weights |c\_k| decay as k^(−1−α). For α = 1 only c\_1 = −1 survives and the update reduces to the forward-Euler LIF neuron.

### Linearization

Around a stationary operating point, each neuron's spike output is replaced by its linear response, s\_j ≈ r\_j + γ\_j (V\_j − V̄\_j), where r\_j is its mean rate and γ\_j its f–I gain. Dropping the threshold gives the **linear FOR**:

```latex
x_n = \Delta t^{\alpha}\left( A\, x_{n-1} + B\, u_n + \beta \right) - \sum_{k=1}^{L} c_k\, x_{n-k}, \qquad A = -\tfrac{1}{\tau_m} I + \tilde W,\quad \tilde W = W\,\mathrm{diag}(\gamma)
```

In the continuous limit (Δt → 0, L → ∞) this is D^α x = A x + B u + β. Silent neurons have γ\_j = 0 and drop out of W̃ automatically, which already says that removing a silent neuron is harmless except through the readout.

### Three variants of "set its weights to zero"

"All of its edge weights" can mean incoming, outgoing or both, and the variants behave differently. The analysis treats full removal as the primary fault and decomposes it into the other two.

| Variant | What is zeroed | What neuron j does afterwards | Error channels |
| --- | --- | --- | --- |
| F-out (silenced) | Column j of W (outgoing synapses) | Still driven by input and still read out | Propagated only |
| F-in (deafened) | Row j of W and of W\_in (incoming) | Relaxes to a bias-driven state that carries no input information; may fire periodically if b exceeds the rheobase | Direct and propagated |
| F-all (removed) | Row and column j, row j of W\_in, readout weight w\_j | Gone | Direct and propagated |

The **direct channel** is what the readout loses by no longer seeing neuron j. The **propagated channel** is the change in every other neuron's state because neuron j stopped talking to them.

## The fault as a forcing term

The fault's whole effect on the surviving neurons is the response of the faulty network to the negative of neuron j's former output. Because the nominal and faulty runs share their entire history up to the fault time t\_f, that response starts from zero history, which removes the usual difficulty with initial conditions in fractional systems.

### Perturbation equation

Let x be the nominal trajectory and x' the faulty one, and let δ = x' − x on the surviving neurons. Write A' for A with row and column j deleted, and a\_j for column j of W̃ restricted to the survivors. Subtracting the two linear FOR equations gives, for F-all:

```latex
D^{\alpha}\delta(t) = A'\,\delta(t) \; - \; a_j\, x_j(t), \qquad \delta(t) = 0 \;\; \text{for } t < t_f
```

The forcing −a\_j x\_j(t) is exactly what neuron j used to send, with its sign flipped. For F-out the equation is the same and neuron j keeps its readout term. For F-in, x\_j(t) is replaced by the difference between its nominal state and its new input-free state.

### Solution

In continuous time the solution is a convolution with the fractional impulse response, written with the two-parameter Mittag-Leffler function E\_{α,β}:

```latex
\delta(t) = -\int_{t_f}^{t} G(t-s)\, a_j\, x_j(s)\, ds, \qquad G(t) = t^{\alpha-1} E_{\alpha,\alpha}\!\left(A' t^{\alpha}\right)
```

For α = 1, G(t) = e^{A't} and this reduces to the familiar exponential convolution.

The discrete GL scheme that runs on the FPGA has an exact z-domain counterpart. Per eigenmode λ of A', the transfer function from forcing to state is:

```latex
H_{\lambda}(z) = \frac{\Delta t^{\alpha}}{C_L(z) - \Delta t^{\alpha}\,\lambda\, z^{-1}}, \qquad C_L(z) = \sum_{k=0}^{L} c_k z^{-k} \;\xrightarrow{L\to\infty}\; (1 - z^{-1})^{\alpha}
```

The branch point of (1 − z^{−1})^α at z = 1 is what produces power-law tails in time; truncating to a polynomial C\_L removes it.

### Two channels and two components

With the readout held fixed, the output error after an F-all fault is:

```latex
e(t) = y'(t) - y(t) = \underbrace{-\,w_j\, x_j(t)}_{\text{direct}} \; + \; \underbrace{\textstyle\sum_{i \ne j} w_i\, \delta_i(t)}_{\text{propagated}}
```

The two channels can partly cancel, so the error variance includes a cross term that can be negative.

It also helps to split neuron j's signal into its mean and its fluctuation, x\_j = x̄\_j + x̃\_j:

- **The mean x̄\_j** acts as a step input at t\_f. It shifts the operating point of every downstream neuron; the next two sections describe how and where to.
- **The fluctuation x̃\_j** carries input information. Its removal acts like injecting structured, input-correlated noise. This component determines the information loss in the information-theory section.

## Recovery dynamics

A permanent fault relaxes toward its new state as t^(−α), and a transient fault decays as t^(−1−α). Both are power laws with no characteristic time. On real hardware the GL buffer length L caps them, at about 2.5L steps at most.

### Permanent fault: step forcing

For one eigenmode λ of A' and a constant forcing F (the mean part of neuron j's lost signal), the response is the fractional step response:

```latex
\delta(t) = \delta^{*}\left[ 1 - E_{\alpha}\!\left(\lambda (t - t_f)^{\alpha}\right) \right], \qquad \delta^{*} = -F/\lambda, \qquad E_{\alpha}(\lambda t^{\alpha}) \approx \frac{-1}{\Gamma(1-\alpha)\,\lambda\, t^{\alpha}} \;\; (t \to \infty)
```

Setting the relative distance to the new state equal to ε gives the ε-recovery time. For α < 1 it scales as a power of 1/ε, not as its logarithm:

```latex
T_{\varepsilon}(\alpha < 1) \approx \left( \varepsilon\, |\lambda|\, \Gamma(1-\alpha) \right)^{-1/\alpha}, \qquad T_{\varepsilon}(\alpha = 1) = |\lambda|^{-1} \ln(1/\varepsilon)
```

For τm = 10 steps (|λ| = 0.1) and ε = 1%, the integer-order neuron recovers in about 46 steps. An untruncated α = 0.5 neuron would need about 318,000. The accumulated excess error ∫|δ − δ\*| dt diverges for every α < 1, because t^(−α) is not integrable.

### Transient fault: pulse forcing

If neuron j is removed for a short duration d and then restored (a transient fault, or a crude stand-in for an upset), the forcing is a pulse and the response is the impulse response G:

```latex
\delta(t) \approx -F d\, G(t - t_f), \qquad G(t) \sim \frac{t^{-1-\alpha}}{\lambda^{2}\, |\Gamma(-\alpha)|} \;\; (t \to \infty)
```

This tail is integrable, so a transient fault costs finite cumulative error. The cost is still heavy-tailed compared with the exponential e^{λt} of an integer-order neuron.

### What truncation changes

The FPGA keeps only L past samples. Truncation leaves a positive residual in the coefficient sum, which acts as an extra leak per step:

```latex
\varepsilon_L = \sum_{k=0}^{L} c_k = \frac{\Gamma(L+1-\alpha)}{\Gamma(1-\alpha)\,\Gamma(L+1)} \approx \frac{L^{-\alpha}}{\Gamma(1-\alpha)}, \qquad \lambda \;\to\; \lambda - \varepsilon_L / \Delta t^{\alpha} \;\; \text{(at DC)}
```

So the power law holds for n ≪ L, after which decay turns exponential. For α = 0.5 and L = 64, ε\_L ≈ 0.07, comparable to the 0.1 leak of τm = 10 steps. The predicted steady-state gain drops from 10 to 5.87, and simulation of Algorithm 1 gives 5.87.

&#91;embedded content: Simulated scalar GL-FLIF step response, Algorithm 1 without threshold, Δt = 1, τm = 10 steps\]

With a long buffer (L = 1024), lower α always means slower recovery. With a short buffer (L = 64), the truncation leak dominates at low α, so recovery time peaks near α ≈ 0.5. In every case it stays below about 2.5L steps, against the untruncated 318,000.

### Check in a network

The exponents were checked in a linear GL network of 100 neurons with random Gaussian coupling (gain 0.8), τm = 2 steps, L = 6000, and an F-all fault at step 3000. The transient fault removed the neuron for 20 steps. For the permanent fault, the slope of dδ/dt was fitted, which should scale as t^(−1−α). Fits used 300 to 2500 steps after the fault.

| α | Predicted exponent | Permanent fault, fitted | Transient fault, fitted |
| --- | --- | --- | --- |
| 0.4 | −1.4 | −0.95 | −1.20 |
| 0.6 | −1.6 | −1.54 | −1.63 |
| 0.8 | −1.8 | −1.81 | −1.90 |

The α = 0.4 mismatch is expected: the asymptotic form needs |λ| t^α ≫ 1, that is t ≫ |λ|^(−1/α) ≈ 316 steps for the slowest mode, which is where the fit window begins. Low-α experiments will need long observation windows to see the predicted tail.

## Where the reservoir settles

The fault's shift of the mean operating point does not depend on α, but its effect on how the reservoir responds to inputs does. Removing a random neuron also slightly shrinks the reservoir's spectrum, pushing it toward the ordered regime.

### The mean shift is independent of α

The fractional derivative of a constant is zero (in the GL form because Σ c\_k = 0), so every α has the same fixed point. For constant forcing, the new steady state of the survivors is:

```latex
\delta^{*} = A'^{-1} a_j\, \bar x_j \quad (L \to \infty), \qquad \delta^{*}_L = \left(A' - \tfrac{\varepsilon_L}{\Delta t^{\alpha}} I\right)^{-1} a_j\, \bar x_j \quad (\text{truncated})
```

In words: α changes the path to the new operating point, not the point itself, unless the buffer is truncated. On the FPGA, α and L reach the endpoint only through the effective leak ε\_L.

The fluctuations are different. The surviving reservoir's frequency response is H'(ω) = ((iω)^α I − A')^(−1), which depends on α at every frequency except zero. So the information-carrying part of the fault's effect is α-dependent even though the mean shift is not.

### Spectral effect of removing a neuron

For a specific neuron, first-order perturbation theory gives the shift of each eigenvalue λ\_i, using its left and right eigenvectors v\_i and u\_i and the change ΔA from deleting row and column j:

```latex
\delta\lambda_i \approx \frac{v_i^{*}\, \Delta A\, u_i}{v_i^{*} u_i}, \qquad |\delta\lambda_i| \le \kappa(U)\, \|\Delta A\| \le \kappa(U)\left( \|\tilde W_{:,j}\| + \|\tilde W_{j,:}\| \right)
```

The bound is Bauer–Fike, with κ(U) the condition number of the eigenvector matrix. Neurons with large incoming and outgoing weight norms, and large components in the slowest modes, are the ones whose removal moves the spectrum most.

For a random reservoir the average effect is simple. If W̃ has i.i.d. entries of variance g²/N, its eigenvalues fill a disk of radius g (the circular law). Deleting k neurons leaves an (N − k)-square matrix with the same entry variance, so:

```latex
g_{\text{eff}} = g \sqrt{1 - p}, \qquad p = k/N
```

Random dropout therefore makes the reservoir more stable and more strongly damped. A reservoir tuned near the edge of chaos (g ≈ 1, Sompolinsky et al. 1988) drifts into the ordered regime as neurons are lost, and memory capacity should fall with it. A single removal changes g by only about g/(2N), so this matters for cumulative faults, not single ones.

### Stability

The continuous linear FOR is stable when every eigenvalue satisfies |arg λ| > απ/2 (Matignon's criterion). Smaller α widens the stable sector into the right half-plane, so a fault that pushes a complex eigenvalue pair rightward can leave a low-α reservoir stable when an integer-order one would not be.

This protection does not extend to real eigenvalues. A real eigenvalue crossing zero destabilizes every α, and for circular-law reservoirs the first crossing is on the real axis at g = 1. So α does not move the critical gain of a random reservoir; it only adds margin against faults that act on oscillatory modes.

Near criticality α does amplify the fault's effect on time scales. A mode near zero relaxes on the time scale |λ|^(−1/α), so:

```latex
\frac{d \ln T_{\lambda}}{d \ln |\lambda|} = -\frac{1}{\alpha}
```

A fault that shifts a slow eigenvalue by a given fraction changes that mode's time scale by twice that fraction at α = 0.5, and by the same fraction at α = 1.

In a chaotic reservoir (g > 1) linear response breaks down entirely, and single-neuron clamping can be catastrophic (Vincent-Lamarre et al. 2016). This analysis assumes the input-driven, echo-state regime.

## Information-theoretic consequences

In the linear, noise-free limit with a retrained readout, removing a neuron costs exactly one unit of memory capacity, whatever the value of α. Every α-dependence in the fault tolerance of Mastin et al.'s three measures therefore comes from noise, nonlinearity or truncation. In all three, the key quantity is the neuron's **unique variance**, Var(x\_j | x\_−j): the part of its state that no other neuron predicts.

The results below assume the linear FOR with Gaussian input or a large-N Gaussian approximation, so that Mastin et al.'s measures (their Eqs. 8, 11 and 12) have closed forms. C = Cov(x) is the state covariance and σ\_u² the input variance.

### Memory capacity, readout retrained

Linear memory capacity at lag k is MC\_k = r\_kᵀ C⁻¹ r\_k / σ\_u², with r\_k = Cov(x\_t, u\_{t−k}). Dropping neuron j as a regressor costs exactly:

```latex
\Delta MC_k^{\text{readout}} = \frac{\beta_{kj}^{2}}{(C^{-1})_{jj}\,\sigma_u^{2}} = \frac{\beta_{kj}^{2}\; \mathrm{Var}(x_j \mid x_{-j})}{\sigma_u^{2}}, \qquad \beta_k = C^{-1} r_k
```

That is the neuron's readout weight for lag k squared, times its unique variance. Summing over lags, with S = Σ\_k r\_k r\_kᵀ / σ\_u² the input-explained part of C:

```latex
\sum_k \Delta MC_k^{\text{readout}} = \frac{e_j^{\top} C^{-1} S\, C^{-1} e_j}{(C^{-1})_{jj}} \;\le\; 1, \qquad \text{equality when } S = C
```

S = C holds for a noise-free linear reservoir driven by i.i.d. input. There the faulty (N − 1)-neuron network also generically keeps MC = N − 1, so the total loss including the propagated channel is exactly 1, independent of α. With state noise (for example fixed-point quantization), S < C and the loss falls below 1 by an α-dependent amount.

A numerical check on six random linear GL networks (N = 10, gain 0.7, τm = 4 steps, L = 400, state noise σ = 10⁻³, 300 lags, one random neuron removed):

| α | MC, intact | Loss, readout only | Loss, whole faulty network | Relative loss |
| --- | --- | --- | --- | --- |
| 1.0 | 6.32 | 0.18 ± 0.10 | 0.41 ± 0.38 | 6.4% |
| 0.7 | 5.30 | 0.08 ± 0.04 | 0.24 ± 0.16 | 4.4% |
| 0.5 | 4.78 | 0.05 ± 0.02 | 0.23 ± 0.16 | 4.8% |

Without noise, one network lost 1.01, 0.95 and 0.79 units at α = 1, 0.7 and 0.5. The low-α shortfall from 1 is memory held at lags beyond the 300-lag window, as predicted.

Two things stand out. First, the propagated channel costs 2 to 5 times more than the readout channel, and the ratio grows as α falls, consistent with long memory spreading the fault through the network. Second, lower α loses less capacity in absolute terms but also starts with less, and the relative difference is within the spread. Six small networks are only indicative.

### Output error, readout fixed

In the online setting the readout is not retrained, so what matters is the output error. After the transient, its power is:

```latex
\mathbb{E}[e^{2}] = \int \left| w'^{\top} H'(\omega) B' - w^{\top} H(\omega) B \right|^{2} S_u(\omega)\, \frac{d\omega}{2\pi} \; + \; \left( w'^{\top} \delta^{*} - w_j \bar x_j \right)^{2}
```

The second term is the bias from the mean shift and is α-independent for an untruncated buffer. During the transient it is replaced by (w'ᵀδ(t) − w\_j x̄\_j)², which carries the power-law tail of the recovery dynamics.

### Information transfer

By the chain rule of mutual information, Mastin et al.'s T = I(U\_past; X\_t) splits cleanly into a direct and a propagated loss:

```latex
\Delta T = \underbrace{I(U_{\text{past}}; X_j \mid X_{-j})}_{\text{direct: unique information}} + \underbrace{I(U_{\text{past}}; X_{-j}) - I(U_{\text{past}}; X'_{-j})}_{\text{propagated}}
```

In the Gaussian model the direct term has a closed form, using C\_ξ = Cov(x | U), the noise covariance:

```latex
I(U_{\text{past}}; X_j \mid X_{-j}) = \tfrac{1}{2} \ln \frac{\mathrm{Var}(x_j \mid x_{-j})}{\mathrm{Var}(x_j \mid x_{-j}, U_{\text{past}})} = \tfrac{1}{2} \ln \frac{(C_{\xi}^{-1})_{jj}}{(C^{-1})_{jj}}
```

The same unique variance governs both this term and the memory-capacity loss. The propagated term has no fixed sign: removing a neuron also removes the noise it injected into the others.

### Active information storage

For a stationary Gaussian reservoir, AIS = I(X\_t; X\_{t+1}) is:

```latex
\mathrm{AIS} = \tfrac{1}{2} \ln \frac{\det C_0}{\det\!\left( C_0 - C_1 C_0^{-1} C_1^{\top} \right)}, \qquad C_1 = \mathrm{Cov}(X_{t+1}, X_t)
```

Two cautions apply. During the recovery transient the reservoir is not stationary, and a deterministic drift is itself predictable, so windowed AIS estimates will be inflated; subtract the mean trajectory (estimated across input realizations) first. And because dropout lowers the effective coupling, g√(1 − p), it weakens the recurrent dominance Mastin et al. associate with high AIS. Cumulative dropout should therefore lower AIS and shift the reservoir toward the input-driven end of their trade-off. That is a hypothesis to test, not a result.

### Redundancy and the correlation peak

Mastin et al. report that neuron-to-neuron correlations peak in the transition region of α. Higher correlation means smaller unique variance, so the theory predicts that the **readout channel** of a single-neuron fault is cheapest near that transition. Strong correlation also implies strong coupling, which enlarges the **propagated channel**. Which effect wins is an open question, and measuring both channels separately across α would answer it directly.

## Spiking-specific effects

The linear theory holds while every neuron stays in its operating regime. Thresholds add three effects it cannot capture: neurons switching on or off, cascades of such switches, and a deafened neuron that keeps firing.

### Threshold crossings

Removing neuron j changes the mean drive of each postsynaptic neuron i by −W\_ij r\_j. If the distance from i's mean potential to threshold is smaller than that change (in the drive's units), i's firing rate changes nonlinearly: it can go silent after losing excitation, or start firing after losing inhibition. Its gain γ\_i then jumps, which changes W̃ itself, not just the state. Linear response is valid only when |W\_ij r\_j| is small compared with the spread of distances to threshold.

### Silencing cascades

Treat each switch as a branching event. Let m be the expected number of neurons that switch as a direct result of one switch; m depends on the degree, the weight distribution and the distribution of distances to threshold. The expected total number of neurons affected by one fault is then:

```latex
\mathbb{E}[\text{cascade size}] = \frac{1}{1 - m} \quad (m < 1), \qquad m \ge 1 \;\Rightarrow\; \text{macroscopic failure}
```

At m ≥ 1 a single fault can propagate through the whole network. α enters in two ways. Mastin et al. show that lower α raises the effective rheobase, so how α is normalized (bias or input gain) sets the distance-to-threshold distribution and therefore m. And because a fractional neuron answers a step change in drive with a power-law approach, a cascade unfolds more slowly at low α: each generation of switches takes longer to complete.

### The deafened neuron keeps firing

Under F-in, neuron j loses all input, but two things keep it active:

- **Its GL buffer still holds pre-fault potentials.** Reset writes V\_reset into the buffer but leaves older samples in place, so j's potential relaxes toward its bias-driven equilibrium along the fractional step response. It goes on spiking for a while, in an echo of its pre-fault activity that fades as a power law, capped by L.
- **Its bias may exceed the rheobase.** If b alone can drive j to threshold, it settles into regular, input-independent firing. The fault then acts as a periodic drive on its downstream neurons, which is narrowband interference rather than silence, and the readout sees a periodic component.

So "deafening" a fractional neuron is neither instantaneous nor necessarily silent. Whether F-in or F-all is the better model of a real hardware failure depends on where in the circuit the failure sits.

### Which neurons matter

Combining the linear results with the spiking picture gives a vulnerability ranking that can be computed from the golden run, before any fault is injected:

- **Direct channel, readout fixed:** |w\_j| · std(x\_j).
- **Direct channel, readout retrained:** β\_j² · Var(x\_j | x\_−j), from the memory-capacity identity.
- **Propagated channel:** r\_j · γ\_j · ‖W\_:,j‖, weighted by the slow-mode components of A'.
- **Cascade risk:** the number of postsynaptic neurons within |W\_ij r\_j| of threshold.

Silent neurons (r\_j = 0, γ\_j = 0) are harmless to remove unless the readout depends on them.

## Testable predictions and analysis plan

The theory yields seven predictions that can be checked against golden-run versus faulty-run differences. The HiL rig produces exactly the quantity the theory is written in, δ(t) = x'(t) − x(t), because it can replay the same input with and without the fault.

| # | Prediction | How to test | What would falsify it |
| --- | --- | --- | --- |
| P1 | After a permanent fault, \|δ(t) − δ\*\| decays as t^(−α) up to about L steps, then exponentially | Log-log slope of dδ/dt (about −1 − α) over a window starting after \|λ\|^(−1/α) steps | Slope independent of α, or no break near L |
| P2 | A transient fault decays as t^(−1−α) | Same fit on a fault of a few steps' duration | As P1 |
| P3 | The 1% recovery time is capped near 2.5L, and peaks at intermediate α for short buffers | Sweep α and L; compare with the recovery-time chart | Recovery time that grows past L, or a monotone curve at L = 64 |
| P4 | The mean shift δ\* is independent of α, apart from the effective leak ε\_L | Compare δ\* across α at fixed L, with and without the ε\_L correction | Residual α-dependence after correction |
| P5 | With a retrained readout, a noise-free linear FOR loses exactly 1 unit of MC per neuron, for every α | Linear GL simulation with long lag windows | Loss that differs from 1 beyond finite-sample bias |
| P6 | Per-neuron damage ranks by the vulnerability scores in the spiking section | Spearman correlation between predicted score and measured loss, over all N single-neuron faults | No correlation |
| P7 | Cumulative random dropout lowers the effective gain as g√(1 − p) and memory capacity falls with it | Fit the leading eigenvalues of the faulty linearized network as p grows | Spectral radius that does not shrink with p |

### Plan

1. **Linear GL network in software.** Confirm P1–P5 and P7 exactly. This is cheap, and every deviation in the later stages can be attributed to spiking or hardware rather than to the theory itself.
2. **Spiking GL-FLIF network in software.** Repeat the same faults with Mastin et al.'s model and normalization. Measure where the linear predictions break, estimate the branching ratio m, and test P6. Separate the direct and propagated channels by also running F-out.
3. **Fixed-point model.** Quantize states and coefficients to the FPGA's formats. Quantization noise enters as state noise, which per the information-theory section is exactly what makes the memory-capacity loss depend on α.
4. **HiL.** Inject F-all at a logged cycle, record all neuron states from both the golden and faulty runs, and compute δ(t), recovery times and the information measures. With the hardware L fixed, P3 says which α values give the slowest recovery.

Use the same estimators as Mastin et al. for MC, AIS and T so that results compare directly with their Figs. 2 and 3. Their paper does not say which estimator was used for AIS and T, so that needs checking with the authors or their code.

## Limitations and open questions

The closed-form results are exact for the linear FOR and approximate for the spiking one. The main gaps, and what each one affects:

- **Linearization.** All closed forms assume small perturbations around a stationary asynchronous state. They fail for neurons near threshold, for cascades with m close to 1, and in the chaotic regime.
- **The effective L is not just the buffer length.** Mastin et al. (Table 1) show that coefficient precision also truncates the history. With UQ0.16 coefficients the usable history is 699 steps at α = 0.5 and 99 at α = 0.9. Use whichever of buffer length and precision limit is smaller when computing ε\_L.
- **Discrete-time stability.** Matignon's criterion is for continuous time. The explicit GL scheme of Algorithm 1 has its own step-size condition, which should be checked for the Δt and τm used on the FPGA.
- **Slow asymptotics at low α.** The power-law tails appear only after |λ|^(−1/α) steps, which can exceed L. At low α the truncated, pre-asymptotic regime may be the only one that is ever observed.
- **Gaussian information measures.** The closed forms for T and AIS assume Gaussian statistics. Spike-train measures need an estimator (binning, Kraskov-type, or a Gaussian approximation on filtered rates), and the estimator Mastin et al. used is not stated in their paper.
- **Small numerical checks.** The memory-capacity table rests on six networks of ten neurons. It shows the direction of the effects, not their size.
- **Fault scope.** Only whole-neuron removal is treated. Faults in the GL coefficient memory change α for every neuron sharing it and need their own analysis; the same perturbation framework applies, with ΔA replaced by a change in the kernel C\_L(z).

The most useful open question for the thesis is the one from the redundancy discussion: across α, does the readout channel shrinking near the correlation peak outweigh the propagated channel growing there?

## Sources

Every entry below has been checked against the publisher, a library catalogue or the cited paper itself. Mastin et al. was read in full; the two project papers are in this project.

1. [Mastin, Anderson, McNeal, Tubbin & Teuscher (2026), "Fractional-order systems for neuromorphic computing: software and hardware opportunities and challenges," npj Unconventional Computing 3:24](https://doi.org/10.1038/s44335-026-00070-8). GL-FLIF model, Algorithm 1, memory capacity, AIS and information transfer, fixed-point history lengths.
2. [Vincent-Lamarre, Lajoie & Thivierge (2016), "Driving reservoir models with oscillations: a solution to the extreme structural sensitivity of chaotic networks," J. Comput. Neurosci. 41:305–322](https://doi.org/10.1007/s10827-016-0619-3). Single-neuron clamping in chaotic reservoirs.
3. Sompolinsky, Crisanti & Sommers (1988), "Chaos in random neural networks," Phys. Rev. Lett. 61(3):259. Transition to chaos at a critical gain.
4. [Matignon (1996), "Stability results for fractional differential equations with applications to control processing," Computational Engineering in Systems Applications (CESA '96), Lille, France, vol. 2, pp. 963–968](https://arxiv.org/abs/2003.03085). Stability criterion |arg λ| > απ/2; the citation details were checked via the reference list of the linked paper.
5. [Podlubny (1999), Fractional Differential Equations, Mathematics in Science and Engineering vol. 198, Academic Press, San Diego](https://find.library.upatras.gr/Record/44508?lng=en). Mittag-Leffler solutions and their asymptotics.
6. [Garrappa (2015), "Numerical evaluation of two and three parameter Mittag-Leffler functions," SIAM J. Numer. Anal. 53(3):1350–1369](https://doi.org/10.1137/140971191). For computing E\_{α,β} when comparing theory with simulation.
7. [Jaeger (2002), "Short term memory in echo state networks," GMD Report 152, German National Research Center for Information Technology](https://link.springer.com/chapter/10.1007/978-3-319-11179-7_6). Memory capacity bounded by the number of units; citation as given in the linked paper's reference list.
8. [Bauer & Fike (1960), "Norms and exclusion theorems," Numerische Mathematik 2:137–141](https://doi.org/10.1007/BF01386217). Eigenvalue perturbation bound.
9. [Tao & Vu, with an appendix by Krishnapur (2010), "Random matrices: universality of ESDs and the circular law," Ann. Probab. 38(5)](https://doi.org/10.1214/10-AOP534). Circular law for i.i.d. matrices.
10. [Lizier, Prokopenko & Zomaya (2012), "Local measures of information storage in complex distributed computation," Information Sciences 208:39–54](https://doi.org/10.1016/j.ins.2012.04.016). Definition and estimation of AIS.
