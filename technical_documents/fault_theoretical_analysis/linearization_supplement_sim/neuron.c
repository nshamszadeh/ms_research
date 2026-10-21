/* Single GL-FLIF neuron, used for the figures and numbers in
   linearization_supplement.tex and Section 1.2 of neuron_dropout_FOR.tex.

   Update rule: Algorithm 1 of Mastin et al. (Eq. glflif of the main document),
   with dt = 1, L = 64, tau_m = 10, V_rest = V_reset = 0, V_th = 1, b = 0 and
   no refractory period. Input I_n = mu + sigma * xi_n, xi_n iid N(0, 1).
   The neuron's actual potential V (with threshold and reset) and its free
   potential Vf (same input, no threshold or reset) are tracked side by side.

   Usage: neuron MODE mu sigma        (environment: ALPHA, default 0.5)
     trace   120 steps of: n, Vf, V, spike                       (Figure 1)
     fi      mean input, rate without noise, rate with noise     (Figure 2)
     stats   operating point, step-by-step regression slope,
             f'(mu) by central difference and gamma = f'/G_DC
     sine    gain from Vf to rate at input periods P: P, gain    (Figure 3)
     winreg  regression slope of window-averaged spikes on
             window-averaged Vf, for several window lengths
             (environment: NSTEPS, default 8e6; SEED)
     drive   std of the drive a target receives: the spike train through
             the membrane filter, against the linear model's prediction
             gamma*(Vf - mean) through the same filter */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define L 64
#define MAXN 8000000L

static double A = 0.5, TAU = 10.0, VTH = 1.0;
static double c[L + 1];
static unsigned long long rng = 88172645463325252ULL;

static double unif(void) {
    rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17;
    return (rng >> 11) * (1.0 / 9007199254740992.0);
}
static double gauss(void) {
    double u = unif(), v = unif();
    if (u < 1e-300) u = 1e-300;
    return sqrt(-2 * log(u)) * cos(2 * M_PI * v);
}

/* last L potentials, index 0 = newest */
typedef struct { double V[L + 1], Vf[L + 1]; } st_t;

static void push(double *h, double v) { memmove(h + 1, h, L * sizeof *h); h[0] = v; }
static double gl(const double *h) {
    double s = 0;
    for (int k = 1; k <= L; k++) s += c[k] * h[k - 1];
    return s;
}

/* one step of both potentials; returns the spike (0 or 1) */
static int step(st_t *s, double I, double *V, double *Vf) {
    double vf = (-s->Vf[0] / TAU + I) - gl(s->Vf);
    double v  = (-s->V[0]  / TAU + I) - gl(s->V);
    int sp = 0;
    if (v >= VTH) { v = 0.0; sp = 1; }
    push(s->Vf, vf); push(s->V, v);
    *V = v; *Vf = vf;
    return sp;
}

static double rate(double mu, double sigma, long burn, long n) {
    st_t s; memset(&s, 0, sizeof s);
    double V, Vf; long cnt = 0;
    for (long i = 0; i < burn + n; i++) {
        int sp = step(&s, mu + sigma * gauss(), &V, &Vf);
        if (i >= burn) cnt += sp;
    }
    return (double)cnt / n;
}

static double xs[MAXN];
static char sps[MAXN];

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s trace|fi|stats|sine|winreg mu sigma\n", argv[0]); return 1; }
    double mu0 = atof(argv[2]), sig0 = atof(argv[3]);
    if (getenv("ALPHA")) A = atof(getenv("ALPHA"));
    c[0] = 1;
    for (int k = 1; k <= L; k++) c[k] = c[k - 1] * (1 - (A + 1) / k);
    double epsL = 0;
    for (int k = 0; k <= L; k++) epsL += c[k];
    double Gdc = 1.0 / (1.0 / TAU + epsL);

    if (!strcmp(argv[1], "trace")) {
        st_t s; memset(&s, 0, sizeof s); double V, Vf;
        for (int i = 0; i < 2000; i++) step(&s, mu0 + sig0 * gauss(), &V, &Vf);
        for (int i = 0; i < 120; i++) {
            int sp = step(&s, mu0 + sig0 * gauss(), &V, &Vf);
            printf("%d %.4f %.4f %d\n", i, Vf, V, sp);
        }
    } else if (!strcmp(argv[1], "fi")) {
        for (int m = 0; m <= 100; m++) {
            double mu = 0.005 * m;
            printf("%.3f %.4f %.4f\n", mu, rate(mu, 0.0, 3000, 30000), rate(mu, sig0, 3000, 100000));
        }
    } else if (!strcmp(argv[1], "stats")) {
        long N = 4000000; st_t s; memset(&s, 0, sizeof s); double V, Vf;
        for (int i = 0; i < 5000; i++) step(&s, mu0 + sig0 * gauss(), &V, &Vf);
        double sx = 0, sxx = 0, ss = 0, sxs = 0;
        for (long i = 0; i < N; i++) {
            int sp = step(&s, mu0 + sig0 * gauss(), &V, &Vf);
            sx += Vf; sxx += Vf * Vf; ss += sp; sxs += Vf * sp;
        }
        double mx = sx / N, vx = sxx / N - mx * mx, r = ss / N, greg = (sxs / N - mx * r) / vx;
        double h = 0.02;
        double fp = (rate(mu0 + h, sig0, 5000, 4000000) - rate(mu0 - h, sig0, 5000, 4000000)) / (2 * h);
        printf("alpha=%.2f epsL=%.4f Gdc=%.4f mu0=%.3f sigma=%.3f\n", A, epsL, Gdc, mu0, sig0);
        printf("r=%.4f meanVf=%.4f (Gdc*mu0=%.4f) sdVf=%.4f\n", r, mx, Gdc * mu0, sqrt(vx));
        printf("step-by-step regression slope=%.4f  fprime=%.4f  gamma=fprime/Gdc=%.4f\n", greg, fp, fp / Gdc);
    } else if (!strcmp(argv[1], "sine")) {
        /* add 0.02*sin(2*pi*n/P) to the mean input; gain = |rate component| / |Vf component| at that period */
        double P[] = {20000, 5000, 2000, 1000, 500, 300, 200, 100, 50, 30, 20, 10, 5}, amp = 0.02;
        for (int j = 0; j < 13; j++) {
            long N = (long)(P[j] * (long)(4000000 / P[j]));
            st_t s; memset(&s, 0, sizeof s); double V, Vf;
            for (int i = 0; i < 5000; i++) step(&s, mu0 + sig0 * gauss(), &V, &Vf);
            double w = 2 * M_PI / P[j], ss = 0, sc = 0, vs = 0, vc = 0;
            for (long i = 0; i < N; i++) {
                double I = mu0 + amp * sin(w * i) + sig0 * gauss();
                int sp = step(&s, I, &V, &Vf);
                ss += sp * sin(w * i); sc += sp * cos(w * i);
                vs += Vf * sin(w * i); vc += Vf * cos(w * i);
            }
            printf("%.0f %.4f\n", P[j], sqrt(ss * ss + sc * sc) / sqrt(vs * vs + vc * vc));
        }
    } else if (!strcmp(argv[1], "winreg")) {
        long N = getenv("NSTEPS") ? atol(getenv("NSTEPS")) : MAXN;
        if (N > MAXN) N = MAXN;
        if (getenv("SEED")) rng = strtoull(getenv("SEED"), 0, 10);
        st_t s; memset(&s, 0, sizeof s); double V, Vf;
        for (int i = 0; i < 5000; i++) step(&s, mu0 + sig0 * gauss(), &V, &Vf);
        for (long i = 0; i < N; i++) { sps[i] = step(&s, mu0 + sig0 * gauss(), &V, &Vf); xs[i] = Vf; }
        long Ws[] = {1, 10, 100, 300, 1000, 3000};
        for (int k = 0; k < 6; k++) {
            long W = Ws[k], M = N / W; double sx = 0, sxx = 0, ss = 0, sxs = 0;
            for (long b = 0; b < M; b++) {
                double ax = 0, as = 0;
                for (long i = b * W; i < (b + 1) * W; i++) { ax += xs[i]; as += sps[i]; }
                ax /= W; as /= W;
                sx += ax; sxx += ax * ax; ss += as; sxs += ax * as;
            }
            double mx = sx / M, vx = sxx / M - mx * mx, cov = sxs / M - mx * ss / M;
            printf("window %5ld: slope %.4f  (windows %ld)\n", W, cov / vx, M);
        }
    } else if (!strcmp(argv[1], "drive")) {
        long N = 4000000; st_t s; memset(&s, 0, sizeof s); double V, Vf;
        for (int i = 0; i < 5000; i++) step(&s, mu0 + sig0 * gauss(), &V, &Vf);
        double ms = 0, mx = 0;
        for (long i = 0; i < N; i++) { sps[i] = step(&s, mu0 + sig0 * gauss(), &V, &Vf); xs[i] = Vf; ms += sps[i]; mx += Vf; }
        ms /= N; mx /= N;
        double h = 0.02;
        double g = (rate(mu0 + h, sig0, 5000, 4000000) - rate(mu0 - h, sig0, 5000, 4000000)) / (2 * h) / Gdc;
        /* both signals through the membrane filter, with the one-step synaptic delay */
        double za[L + 1] = {0}, zl[L + 1] = {0}, sa = 0, saa = 0, sl = 0, sll = 0; long M = 0;
        for (long i = 1; i < N; i++) {
            double a = (-za[0] / TAU + (sps[i - 1] - ms)) - gl(za);
            double l = (-zl[0] / TAU + g * (xs[i - 1] - mx)) - gl(zl);
            push(za, a); push(zl, l);
            if (i > 5000) { sa += a; saa += a * a; sl += l; sll += l * l; M++; }
        }
        double sda = sqrt(saa / M - sa * sa / M / M), sdl = sqrt(sll / M - sl * sl / M / M);
        printf("alpha=%.2f gamma=%.4f  std filtered spikes %.4f  std filtered linear prediction %.4f  ratio %.2f\n",
               A, g, sda, sdl, sda / sdl);
    } else {
        fprintf(stderr, "unknown mode %s\n", argv[1]);
        return 1;
    }
    return 0;
}
