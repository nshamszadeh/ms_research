# Literature Review Plan: Fault Tolerance of Fractional-Order Reservoir Computers

*Working plan for the thesis literature review. Last updated 10 October 2026.*

This document sets out how the literature review is organized, how papers are found, read and recorded, and how the chapter gets written. It is a living plan: update the checklists, the concept matrix and the search log as the review progresses.

---

## 1. Organizing principle

The review is organized around the **gap the thesis fills**, not around individual papers or chronology.

The thesis sits where three literatures meet, with hardware running through all of them:

- **Reservoir and spiking computing**: how reservoirs compute, what they remember, how they are built.
- **Fractional-order dynamics**: fractional calculus, fractional-order neurons, fractional-order reservoirs.
- **Fault tolerance**: how faults are defined, modeled, injected and measured, in general and in neural networks.
- **Hardware** (cross-cutting): FPGA implementations, radiation effects, hardware-in-the-loop fault injection.

Each literature is well developed on its own, and some pairs overlap (fault tolerance of reservoirs; fractional-order reservoirs). The centre, **fault tolerance of fractional-order reservoirs, analyzed theoretically and tested on hardware**, is close to empty. The review's job is to walk through each area and then show that.

---

## 2. Three kinds of material, three places in the thesis

| Material | Where it goes | Its job |
|---|---|---|
| **Background**: textbook-level concepts the reader needs | Chapter 2, Part A | Teach. Reads like a textbook; no argument. |
| **Related work**: specific prior studies closest to this thesis | Chapter 2, Part B | Position. What each did and did not do, and why that leaves room for this thesis. |
| **Own derivations**, e.g. the neuron-dropout analysis | Separate theory chapter | Contribute. The review describes other people's frameworks; later chapters use them. |

Keeping these apart stops the background from swelling and keeps the argument for the contribution in one clearly visible place.

---

## 3. Chapter outline

Each section lists what it must **establish** (the point a reader should leave with), what it **covers**, and **starter references** (see §9 for full citations). Tick sections off as they reach draft and final.

### 2.1 Scope and structure

- [ ] Draft  - [ ] Final

**Establish:** what the chapter covers and why these threads.
**Covers:** the three threads plus hardware; inclusion criteria (§5.5); what is out of scope, including how deep radiation effects go.
**Length:** about one page.

### Part A: Background

#### 2.2 Reservoir computing

- [ ] Draft  - [ ] Final

**Establish:** how a reservoir computes and how its computational capacity is measured.
**Covers:**
- Echo state networks and liquid state machines; the echo state property.
- Readout training (ridge regression).
- Memory capacity and its bound by the number of units.
- Spiking reservoirs.
- Information-theoretic measures used later: active information storage, information transfer.

**Starter references:** Jaeger (2002); Maass, Natschläger & Markram (2002); Lukoševičius & Jaeger (2009); Lizier, Prokopenko & Zomaya (2012).

#### 2.3 Fractional-order neurons

- [ ] Draft  - [ ] Final

**Establish:** what fractional-order dynamics add (power-law memory), and how they are computed in practice.
**Covers:**
- Riemann–Liouville, Caputo and Grünwald–Letnikov definitions.
- Mittag-Leffler functions; Matignon's stability criterion; the short-memory principle.
- Biological motivation: power-law adaptation in real neurons.
- The fractional LIF neuron.
- Fractional-order reservoirs and the α trade-off between memory and input sensitivity.

**Starter references:** Podlubny (1999); Matignon (1996); Garrappa (2015); Lundstrom et al. (2008); Teka, Marinov & Santamaria (2014); Mastin et al. (2026).

#### 2.4 Dependability and fault tolerance foundations

- [ ] Draft  - [ ] Final

**Establish:** the vocabulary, fault models and metrics the thesis uses, and where they come from.
**Covers:**
- The fault → error → failure chain; masking versus detection and recovery.
- Fault-model taxonomy: stuck-at, bit-flip, crash, omission, timing, Byzantine. Place the thesis's neuron-dropout fault (crash / fail-silent) and radiation upsets (bit-flip) in it.
- Reliability metrics: R(t), hazard rate, MTTF, availability, coverage, k-out-of-n and redundancy.
- Graceful-degradation metrics: performability; the resilience triangle (drop depth, recovery time, loss area).
- Fault modeling in dynamical systems and control: additive versus multiplicative faults, sensitivity analysis.
- Statistical fault injection and the architectural vulnerability factor (AVF).
- Radiation effects (SEU, MBU, SET, configuration upsets), sized to how much of the thesis uses them.

**Starter references:** Avižienis et al. (2004); von Neumann (1956); Bouricius, Carter & Schneider (1969); Meyer (1980); Bruneau et al. (2003); Mukherjee et al. (2003); Leveugle et al. (2009); JEDEC JESD89; Blanke et al. (textbook).

#### 2.5 Linear and linearized analysis of spiking networks

- [ ] Draft  - [ ] Final

**Establish:** that linearization is the standard first-order tool for analyzing perturbations in reservoirs and spiking networks, under what conditions it holds, and how it can be extended. This section supplies the literature behind the theory chapter's linear model (see §7).
**Covers:**
- Linear echo state networks as the analytically tractable case of reservoir computing (memory-capacity results).
- Linearized state-space models in fault diagnosis and fault-tolerant control (cross-reference 2.4).
- Linear response theory for spiking networks: single-neuron susceptibilities from Fokker–Planck methods, and network-level formulas for how rates and correlations respond to perturbations.
- Corrections beyond linear response: loop and finite-size expansions.
- The exactly linear structure of integrate-and-fire models between spikes, and where the nonlinearity enters (threshold and reset). These models are piecewise affine, or hybrid, systems.
- Limits: "stable chaos", where a single extra or missing spike sends the exact spike pattern elsewhere even though the network is linearly stable; the chaotic regime.
- Systematic extensions beyond first order: Volterra series and fading memory; the Koopman operator and extended dynamic mode decomposition; universality of echo state networks for fading-memory filters.

**Starter references:** Jaeger (2002); Brunel & Hakim (1999); Trousdale et al. (2012); Ocker et al. (2017); Jahnke, Memmesheimer & Timme (2008); Boyd & Chua (1985); Williams, Kevrekidis & Rowley (2015); Korda & Mezić (2018); Grigoryeva & Ortega (2018); Blanke et al. (textbook).

### Part B: Related work

#### 2.6 Fault tolerance in neural networks

- [ ] Draft  - [ ] Final

**Establish:** what is known about how neural networks degrade under faults, and that almost all of it concerns feedforward, integer-order networks.
**Covers:**
- Classic weight-perturbation sensitivity and stuck-at neuron studies.
- Theoretical error-propagation bounds for crashed and Byzantine neurons.
- Reviews of the field.
- Fault injection in DNN accelerators.
- Fault studies of spiking and neuromorphic hardware.

**Starter references:** El Mhamdi & Guerraoui (2017); Torres-Huitzil & Girau (2017); Reagen et al. (2018).

#### 2.7 Structural robustness of recurrent networks and reservoirs

- [ ] Draft  - [ ] Final

**Establish:** that recurrent reservoirs can be robust to noise yet fragile to structural damage, and what is known about making them robust.
**Covers:**
- The transition to chaos in random networks.
- Single-neuron clamping in chaotic reservoirs; local versus global perturbations.
- Graceful degradation in spiking reservoirs.
- Robustness mechanisms: modularity, oscillatory driving.
- **The lab's Stage B result** (Mackey-Glass prediction; fault-tolerance optimum near α ≈ 0.7; integer order the most fragile). This is the nearest prior result to the thesis, so position against it explicitly.

**Starter references:** Sompolinsky, Crisanti & Sommers (1988); Vincent-Lamarre, Lajoie & Thivierge (2016); Vincent-Lamarre, Calderini & Thivierge (2020); Kawai et al. (2024); the 2025 modular-reservoir injury preprint (authors and DOI still to find).

#### 2.8 Fractional-order systems in hardware

- [ ] Draft  - [ ] Final

**Establish:** how fractional dynamics are realized digitally, and what that costs.
**Covers:**
- FPGA and ASIC implementations of fractional-order neurons and systems.
- The trade-off between coefficient precision and usable history length.
- Any robustness results from fractional-order control.

**Starter references:** Mastin et al. (2026) and the hardware works it cites: the reviews by Clemente-López et al. and Ali et al., and the FPGA neurons of Malik & Mir and Tolba et al. Pull these from its reference list.

#### 2.9 Fault-injection and hardware-in-the-loop methods

- [ ] Draft  - [ ] Final

**Establish:** that the experimental method is grounded in accepted practice.
**Covers:** on-fabric and processor-driven fault injection on FPGAs; configuration-memory injection; hardware-in-the-loop testing of controllers.

**Starter references:** AMD Soft Error Mitigation controller guides (PG036, PG187); Leveugle et al. (2009).

#### 2.10 Synthesis and research gap

- [ ] Draft  - [ ] Final

**Establish:** the gap, and the research questions that follow from it.
**Covers:** the concept matrix (§4) and a short narrative of what it shows; the research questions.

**Draft research questions** (to revise as the review matures):

1. How does fractional order α affect degradation and recovery when neurons fail in a fractional-order reservoir performing closed-loop control?
2. Do the linear theory's predictions (power-law relaxation, relaxation time capped by the GL buffer length, α-independent mean shift) hold in simulation and on FPGA hardware, and where does the theory break down, as measured by spike flips?
3. How do radiation-style bit-flips compare with whole-neuron failure in their effect on the reservoir?

---

## 4. Concept matrix

The concept matrix (Webster & Watson, 2002) is a table with one row per paper and one column per dimension that matters to the thesis. It is both a working tool while reading and the centrepiece of §2.10. Every paper added either fills a cell or shows the gap is narrower than thought, which is better discovered now than at the defense.

### Columns

| Column | What to record |
|---|---|
| Neuron model | Rate or spiking; integer or fractional order |
| Network | Reservoir, recurrent, feedforward |
| Fault model | Clamping/crash, stuck-at, bit-flip, noise, Byzantine, none |
| Task | Prediction, classification, control, generative |
| Platform | Simulation, FPGA, ASIC, HiL |
| Metric | Error/accuracy change, recovery time, AVF, reliability |
| Theory? | Kind of analysis, if any |

### Starter rows

| Work | Neuron model | Fault model | Task | Platform | Theory? |
|---|---|---|---|---|---|
| Vincent-Lamarre et al. 2016 | Chaotic recurrent network | Neuron clamping | Generative | Simulation | Spectral analysis |
| Vincent-Lamarre et al. 2020 | Spiking LIF | Neuron clamping | Generative | Simulation | — |
| El Mhamdi & Guerraoui 2017 | Feedforward | Crash, Byzantine | Generic | — | Error bounds |
| Mastin et al. 2026 | Fractional LIF | None | Classification, control, prediction | Software, SystemVerilog | Information measures |
| Lab Stage B | Fractional LIF | Clamping, p = 0.05 per step | Prediction | Simulation (SPIRES) | — |
| **This thesis** | Fractional LIF | Clamping (+ bit-flips) | Control | FPGA HiL | Perturbation analysis |
| | | | | | |

Keep the matrix in sync with the Zotero tags in §5.1, so a filter on a tag gives the papers in a column.

---

## 5. Workflow

### 5.1 Zotero setup

**Collections** mirror the outline, so every paper has a home section:

```
Thesis LitReview/
  2.2 Reservoir computing
  2.3 Fractional-order neurons
  2.4 Dependability foundations
  2.5 Linearization methods
  2.6 NN fault tolerance
  2.7 Recurrent/reservoir robustness
  2.8 Fractional-order hardware
  2.9 Fault injection & HiL
  _Inbox (unsorted)
  _Excluded (with reason in note)
```

**Tags** map to the concept-matrix columns, with a prefix per column:

```
neuron:rate  neuron:lif  neuron:flif
net:reservoir  net:recurrent  net:feedforward
fault:clamp  fault:stuck-at  fault:bitflip  fault:noise  fault:byzantine  fault:none
task:prediction  task:classification  task:control  task:generative
platform:sim  platform:fpga  platform:asic  platform:hil
theory:linear  theory:bounds  theory:spectral  theory:other
status:skimmed  status:read  status:noted  status:cited
```

### 5.2 Paper note template

Fill one note per paper (a Zotero child note). Writing then becomes assembly.

```markdown
## <Short citation, e.g. Vincent-Lamarre 2016>
**Section:** 2.x
**Main claim:** one or two sentences.
**Method:** model, network size, task, how faults were applied.
**Fault model:**
**Metric:**
**Key result (with numbers):**
**Limitation / what it did not do:**
**Relevance to my thesis:** supports / contrasts / method I reuse / gap it leaves.
**Quotes or figures to cite (page numbers):**
**Follow-up references to chase:**
```

### 5.3 Search strategy

1. **Anchor papers.** Start from Mastin et al. (2026), Vincent-Lamarre et al. (2016), El Mhamdi & Guerraoui (2017) and Avižienis et al. (2004).
2. **Backward chasing.** Go through each anchor's reference list and pull anything relevant into `_Inbox`.
3. **Forward chasing.** Use "cited by" in Google Scholar or Semantic Scholar to find later work building on each anchor.
4. **Keyword searches** per thread, in Google Scholar, IEEE Xplore, Scopus or Web of Science, and arXiv. Starter strings:
   - `"fractional-order" AND ("reservoir computing" OR "spiking neural network")`
   - `"fractional leaky integrate-and-fire"`
   - `"reservoir computing" AND ("fault tolerance" OR robustness OR "neuron failure" OR lesion)`
   - `("spiking neural network" OR neuromorphic) AND ("fault injection" OR "soft error" OR "single event upset")`
   - `"fractional-order" AND FPGA AND (neuron OR "Grünwald-Letnikov")`
   - `"linear response" AND ("spiking network" OR "integrate-and-fire")`
   - `("piecewise linear" OR "hybrid system") AND "integrate-and-fire" AND (perturbation OR Lyapunov)`
   - `Koopman AND ("spiking" OR "reservoir computing")`
   - `"Volterra series" AND "fading memory"`
5. **Stopping rule.** Stop searching a thread when new papers stop adding new cells to the concept matrix (saturation). Record the decision in the search log.

### 5.4 Search log

Keep this table up to date. It answers the defense question "how do you know you've covered the field?"

| Date | Database | Query or anchor | Results screened | Kept | Notes |
|---|---|---|---|---|---|
| | | | | | |

### 5.5 Inclusion and exclusion criteria (draft)

**Include:**
- Peer-reviewed papers, theses and well-cited preprints on any of the three threads.
- Foundational works, regardless of age, when the thesis uses their definitions or metrics.
- Hardware papers when they report implementation details or fault behaviour relevant to FPGA reservoirs.

**Exclude (with the reason noted in `_Excluded`):**
- Security-oriented fault-injection attacks, unless the method is reused.
- Fault tolerance of non-neural systems, unless it supplies a metric or method used in the thesis.
- Fractional-order work with no link to neurons, reservoirs, hardware or robustness.

### 5.6 Reading triage

Read in three passes, and stop at the pass that tells you enough:

1. **Skim** (5–10 min): abstract, figures, conclusions. Decide: keep, exclude or defer. Tag `status:skimmed`.
2. **Read** (about 1 h): method and results. Fill the note template and the matrix row. Tag `status:read` / `status:noted`.
3. **Deep read**: only for the handful of papers you will position against directly (Mastin et al., Vincent-Lamarre et al., Stage B, El Mhamdi & Guerraoui). Check their equations and assumptions.

---

## 6. Writing guidelines

- **Synthesize by theme, not paper by paper.** Write "Three studies clamp neurons, all in generative tasks", not "A did X. B did Y. C did Z."
- **End each section with what it means for the thesis**, in a short paragraph that links to the next section or to the gap.
- **Write §2.10 first, as a draft.** Stating the gap early tells you what every other section has to establish, and what can be cut. Revisit it at the end, because results shift the framing.
- **Weight Part B more than Part A.** The contribution is argued in Part B; the background only needs enough depth for the reader to follow. Check the department's norms for total length.
- **Cite primary sources** for definitions and metrics, not later papers that repeat them.
- **Verify every citation** against the publisher or the paper itself before it goes into the thesis. Several entries in §9 are still marked as not yet checked.

---

## 7. The linear model in the theoretical analysis

The theory chapter analyzes faults with a linear model of the GL-FLIF reservoir. A committee will ask whether that is justified for a spiking network. These notes set out where the argument goes, what it consists of, which validity conditions to state, how to test them, and which questions to expect.

### 7.1 Where each piece goes

| Piece | Location |
|---|---|
| Literature showing linear analysis is standard, and its known limits | Review §2.5 |
| The model, the exactness result, validity conditions and derivations | Theory chapter |
| Tests of the validity conditions and of the predictions' accuracy | Results chapters |

### 7.2 The justification, step by step

1. **Linear analysis is the standard first-order tool.** It is the default in fault diagnosis and fault-tolerant control, in reservoir-computing theory (linear echo state networks) and in spiking-network theory (linear response). Fault-tolerance engineering itself relies mainly on fault injection and probabilistic reliability models, so present the linear theory as complementing fault injection, not replacing it.
2. **The GL-FLIF model is exactly linear between spike changes.** In Algorithm 1 of Mastin et al. (2026), the only nonlinear operation is the threshold test. With the spike raster s fixed, every update is affine:

   ```
   V_n = Δt^α ( −(V_{n−1} − V_rest)/τ_m + W s_{n−1} + W_in u_n + b ) − Σ_k c_k V_{n−k}
   ```

   with V_n set to V_reset at the fixed reset steps. The network is piecewise affine, so the frozen-raster linear model is exact until the first spike flips (appears or disappears relative to the fault-free run). This is a precise validity condition, not a vague "small perturbation" assumption.
3. **The error has an exact decomposition.** The faulty trajectory equals the linear response to the fault's forcing plus one impulse response per flipped spike. Each flip injects ±W_ij into its targets and a reset jump into its own history buffer. Flips propagate in generations (fault → direct targets → their targets → …), which is a finite sum over any finite run.
4. **The rate-level linear model is the average of that sum.** The gain γ_i is the probability that neuron i flips per unit change in its voltage. The network term (I − GW)⁻¹ expands as Σ_k (GW)^k, and the k-th term is the k-th flip generation. The branching ratio m measures whether generations grow or shrink. Linear response is therefore exact on average when flips are rare and independent.
5. **State the regime.** The analysis applies to the input-driven (echo-state) regime, where the input pins the trajectory and divergences die out. It does not cover the chaotic regime, where single-neuron clamping can be catastrophic (Vincent-Lamarre et al. 2016). Exact spike timing can diverge even in linearly stable networks (stable chaos; Jahnke, Memmesheimer & Timme 2008), so frame claims in terms of rates, state averages and their statistics, not exact spike times.
6. **Separate robust predictions from fragile ones.**
   - *Robust:* power-law relaxation exponents and their α-dependence; the cap set by the GL buffer length; the α-independence of the mean shift; the qualitative ordering of fault types.
   - *Fragile:* exact spike times; cascade thresholds (m near 1); anything that depends on neurons sitting close to threshold.
   - *Linear-only:* the identity "exactly one unit of memory capacity lost per removed neuron" holds for linear, noise-free reservoirs. Present it as a reference point, not as a spiking-network result.
7. **Name the extensions, even if you don't use them.** Higher-order expansion in fault size, where each order solves the same linear fractional equation with the same Green's function; Volterra series for fading-memory systems (Boyd & Chua 1985); Koopman operators and EDMD (Korda & Mezić 2018); corrections beyond linear response (Ocker et al. 2017). Naming them shows the linear model is the first term of a systematic approach, not an arbitrary simplification.

### 7.3 Validity conditions to state explicitly

- The fault's change to each postsynaptic neuron's drive, |W_ij r_j|, is small compared with the spread of distances to threshold across the network.
- The reservoir is in the input-driven, contracting regime: the echo state property holds and the coupling gain is below the onset of chaos.
- Spike flips are rare and don't cascade: branching ratio m < 1.
- The operating point is stationary, so the gains γ_i are well defined.
- The finite history length L is included through the effective leak ε_L. Use whichever is smaller, the buffer length or the limit set by coefficient precision.

### 7.4 Validation plan

1. **Frozen-raster check.** In a small software GL-FLIF network, compare the frozen-raster linear prediction with the true faulty run. They must agree exactly until the first flip, which also tests the implementation.
2. **Spike-flip diagnostics.** Count flipped spikes per generation between the fault-free and faulty runs, estimate m, and report both against α, fault size and coupling gain. On the HiL rig, log the spike rasters of both runs.
3. **Accuracy map.** Report the error of the linear prediction against fault size and α, and mark the region where it stays below a chosen tolerance.
4. **Rate-level check.** Measure each neuron's f–I slope to get γ_i, and compare the rate-level linear prediction with network-averaged deviations.

### 7.5 Questions to expect at the defense

| Question | Short answer | Where addressed |
|---|---|---|
| Why is a linear model valid for a spiking network? | It is exact until a spike flips, and flips are counted. | Theory chapter; §7.4 |
| Isn't removing a neuron a large perturbation? | Large for that neuron, but what matters is the change to each target's input relative to its distance from threshold. That is small when weights scale with network size, and flip counts test it. | Theory chapter; results |
| What about chaos? | The analysis is restricted to the input-driven regime; the chaotic case is a stated limitation. | Review §2.5, §2.7; theory chapter |
| Why not just simulate? | The theory predicts scaling laws (exponents, the buffer-length cap, the α-independent mean shift) that simulations then test, and it says which experiments matter. | Theory chapter introduction |
| How accurate is it? | See the accuracy map; higher-order corrections are available if needed. | Results; §7.2 step 7 |
| Is replacing spikes with rates justified? | It is standard linear response theory, and the gains are measured rather than assumed. | Review §2.5; §7.4 step 4 |

---

## 8. Order of work

- [ ] Set up Zotero collections and tags (§5.1).
- [ ] Deep-read the anchor papers; fill their notes and matrix rows.
- [ ] Draft §2.10 (gap and research questions) as a target.
- [ ] Backward and forward chasing from the anchors; log every search (§5.4).
- [ ] Keyword searches per thread until saturation.
- [ ] Draft Part A (2.2–2.5).
- [ ] Draft Part B (2.6–2.9).
- [ ] Write the linear-model justification and validity conditions into the theory chapter (§7.2–7.3).
- [ ] Run the frozen-raster and spike-flip checks (§7.4) before drafting the results chapters.
- [ ] Revise §2.10 against the completed matrix.
- [ ] Verify all citations; resolve every "not yet checked" entry in §9.
- [ ] Final revision after results chapters are drafted.

---

## 9. Starter reading list

**Status key:** *Checked* = details confirmed against the publisher, a library catalogue, the paper itself or a citing paper's reference list. *Project* = PDF is in the Reservoir Computing Research project. *Not yet checked* = cited from memory; verify before use.

| Reference | Section | Status |
|---|---|---|
| Mastin, Anderson, McNeal, Tubbin & Teuscher (2026), "Fractional-order systems for neuromorphic computing: software and hardware opportunities and challenges," *npj Unconventional Computing* 3:24. [doi:10.1038/s44335-026-00070-8](https://doi.org/10.1038/s44335-026-00070-8) | 2.3, 2.8 | Checked |
| Vincent-Lamarre, Lajoie & Thivierge (2016), "Driving reservoir models with oscillations: a solution to the extreme structural sensitivity of chaotic networks," *J. Comput. Neurosci.* 41:305–322. [doi:10.1007/s10827-016-0619-3](https://doi.org/10.1007/s10827-016-0619-3) | 2.7 | Checked, Project |
| Vincent-Lamarre, Calderini & Thivierge (2020), "Learning long temporal sequences in spiking networks by multiplexing neural oscillations," *Front. Comput. Neurosci.* 14:78 | 2.7 | Project |
| Sompolinsky, Crisanti & Sommers (1988), "Chaos in random neural networks," *Phys. Rev. Lett.* 61(3):259 | 2.7 | Checked, Project |
| Jaeger (2002), "Short term memory in echo state networks," GMD Report 152, German National Research Center for Information Technology | 2.2, 2.5 | Checked |
| Maass, Natschläger & Markram (2002), "Real-time computing without stable states: a new framework for neural computation based on perturbations," *Neural Computation*. [PubMed](https://pubmed.ncbi.nlm.nih.gov/12433288/) | 2.2 | Checked |
| Lukoševičius & Jaeger (2009), "Reservoir computing approaches to recurrent neural network training," *Computer Science Review*. [PDF](https://www.ai.rug.nl/minds/uploads/2261_LukoseviciusJaeger09.pdf) | 2.2 | Checked |
| Lizier, Prokopenko & Zomaya (2012), "Local measures of information storage in complex distributed computation," *Information Sciences* 208:39–54. [doi:10.1016/j.ins.2012.04.016](https://doi.org/10.1016/j.ins.2012.04.016) | 2.2 | Checked |
| Podlubny (1999), *Fractional Differential Equations*, Mathematics in Science and Engineering vol. 198, Academic Press | 2.3 | Checked |
| Matignon (1996), "Stability results for fractional differential equations with applications to control processing," CESA '96, Lille, vol. 2, pp. 963–968 | 2.3 | Checked |
| Garrappa (2015), "Numerical evaluation of two and three parameter Mittag-Leffler functions," *SIAM J. Numer. Anal.* 53(3):1350–1369. [doi:10.1137/140971191](https://doi.org/10.1137/140971191) | 2.3 | Checked |
| Teka, Marinov & Santamaria (2014), "Neuronal spike timing adaptation described with a fractional leaky integrate-and-fire model," *PLoS Comput. Biol.* [PMC3967934](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3967934/) | 2.3 | Checked |
| Lundstrom, Higgs, Spain & Fairhall (2008), "Fractional differentiation by neocortical pyramidal neurons," *Nature Neuroscience* | 2.3 | Title checked; volume and pages not yet checked |
| Avižienis, Laprie, Randell & Landwehr (2004), "Basic concepts and taxonomy of dependable and secure computing," *IEEE Trans. Dependable Secure Comput.* [PDF](https://www.inf.ed.ac.uk/teaching/courses/seoc/2006_2007/resources/BasicConcepts.pdf) | 2.4 | Checked |
| von Neumann (1956), "Probabilistic logics and the synthesis of reliable organisms from unreliable components," *Automata Studies*. [PDF](https://static.ias.edu/pitp/archive/2012files/Probabilistic_Logics.pdf) | 2.4 | Checked |
| Bouricius, Carter & Schneider (1969), "Reliability modeling techniques for self-repairing computer systems." [IBM Research](https://research.ibm.com/publications/reliability-modeling-techniques-for-self-repairing-computer-systems) | 2.4 | Checked |
| Meyer (1980), "On evaluating the performability of degradable computing systems," *IEEE Trans. Computers*. [Overview](https://resilinets.org/performability.html) | 2.4 | Checked |
| Bruneau et al. (2003), "A framework to quantitatively assess and enhance the seismic resilience of communities," *Earthquake Spectra*. [doi:10.1193/1.1623497](https://doi.org/10.1193/1.1623497) | 2.4 | Checked |
| Mukherjee, Weaver, Emer, Reinhardt & Austin (2003), architectural vulnerability factors, MICRO-36. [IEEE](https://ieeexplore.ieee.org/document/1253181) | 2.4 | Checked |
| Leveugle, Calvez, Maistri & Vanhauwaert (2009), "Statistical fault injection: quantified error and confidence," DATE 2009. [HAL](https://hal.archives-ouvertes.fr/hal-00386107) | 2.4, 2.9 | Checked |
| JEDEC JESD89B, "Measurement and reporting of alpha particle and terrestrial cosmic ray induced soft errors in semiconductor devices." [JEDEC](https://www.jedec.org/node/8374) | 2.4 | Checked |
| Albert, Jeong & Barabási (2000), "Error and attack tolerance of complex networks," *Nature* 406. [arXiv](https://arxiv.org/abs/cond-mat/0008064) | 2.4 | Checked |
| Brunel & Hakim (1999), "Fast global oscillations in networks of integrate-and-fire neurons with low firing rates," *Neural Computation*. [arXiv](https://arxiv.org/abs/cond-mat/9904278) | 2.5 | Checked |
| Trousdale, Hu, Shea-Brown & Josić (2012), "Impact of network structure and cellular response on spike time correlations," *PLoS Comput. Biol.* [PMC3310711](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3310711/) | 2.5 | Checked |
| Ocker, Josić, Shea-Brown & Buice (2017), "Linking structure and activity in nonlinear spiking networks," *PLoS Comput. Biol.* [PMC5507396](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5507396/) | 2.5 | Checked |
| Jahnke, Memmesheimer & Timme (2008), "Stable irregular dynamics in complex neural networks," *Phys. Rev. Lett.* 100. [arXiv](https://arxiv.org/abs/0705.3214) | 2.5, 2.7 | Checked |
| Boyd & Chua (1985), "Fading memory and the problem of approximating nonlinear operators with Volterra series." [Stanford](https://stanford.edu/~boyd/papers/fading_volterra.html) | 2.5 | Checked (journal details not recorded) |
| Williams, Kevrekidis & Rowley (2015), "A data-driven approximation of the Koopman operator: extending dynamic mode decomposition." [arXiv](https://arxiv.org/abs/1408.4408) | 2.5 | Title checked; journal details not yet checked |
| Korda & Mezić (2018), "On convergence of extended dynamic mode decomposition to the Koopman operator." [arXiv](https://arxiv.org/abs/1703.04680) | 2.5 | Checked (journal details not recorded) |
| Grigoryeva & Ortega (2018), "Echo state networks are universal." [arXiv](https://arxiv.org/abs/1806.00797) | 2.5 | Checked (journal details not recorded) |
| El Mhamdi & Guerraoui (2017), "When neurons fail," IPDPS 2017. [arXiv](https://arxiv.org/abs/1706.08884) | 2.6 | Checked |
| Torres-Huitzil & Girau (2017), "Fault and error tolerance in neural networks: a review," *IEEE Access* 5:17322–17341. [doi:10.1109/ACCESS.2017.2742698](https://doi.org/10.1109/ACCESS.2017.2742698) | 2.6 | Checked |
| Reagen et al. (2018), "Ares: a framework for quantifying the resilience of deep neural networks," DAC 2018. [Harvard](https://vlsiarch.eecs.harvard.edu/publications/ares-framework-quantifying-resilience-deep-neural-networks) | 2.6 | Checked |
| Kawai, Morita, Park & Asada (2024), "Oscillation-driven reservoir computing for long-term replication of chaotic time series," ICANN 2024 | 2.7 | Not yet checked (from project proposal) |
| Modular-architecture injury preprint (2025), bioRxiv | 2.7 | Not yet checked; authors and DOI unknown |
| AMD PG036 and PG187, Soft Error Mitigation controller product guides. [PG036](https://docs.amd.com/r/en-US/pg036_sem/Injection), [PG187](https://docs.amd.com/r/en-US/pg187-ultrascale-sem/Configuration-Memory-Masking) | 2.9 | Checked |
| Webster & Watson (2002), "Analyzing the past to prepare for the future: writing a literature review," *MIS Quarterly*. [PDF](https://ntnu.no/wiki/download/attachments/98080959/Webster%20%20Watson%20-%20Writing%20a%20literature%20review.pdf?api=v2) | Method | Checked |
| Blanke, Kinnaert, Lunze & Staroswiecki, *Diagnosis and Fault-Tolerant Control*, Springer | 2.4, 2.5 | Not yet checked |
| Koren & Krishna, *Fault-Tolerant Systems*, Morgan Kaufmann (introductory textbook) | 2.4 | Not yet checked |
| Winograd & Cowan (1963), *Reliable Computation in the Presence of Noise*, MIT Press | 2.4 | Not yet checked |
| Huang & Abraham (1984), algorithm-based fault tolerance for matrix operations, *IEEE Trans. Computers* | 2.4 | Not yet checked |
| Lab Stage B result (Mackey-Glass fault-tolerance sweep, SPIRES) | 2.7 | Internal; confirm how to cite |
