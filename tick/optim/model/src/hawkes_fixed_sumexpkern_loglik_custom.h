#ifndef TICK_OPTIM_MODEL_SRC_CUSTOM_SUMEXP
#define TICK_OPTIM_MODEL_SRC_CUSTOM_SUMEXP

// License: BSD 3 clause

#include "base.h"

#include "base/hawkes_fixed_sumexpkern_loglik_custom.h"

/**
 * \class ModelHawkesCustom
 * \brief Class for computing loglikelihood function and gradient for Hawkes processes with
 * exponential kernels with fixed exponent (i.e., \f$ \alpha \beta e^{-\beta t} \f$, with fixed
 * decay)
 */
class DLL_PUBLIC ModelHawkesSumExpCustom : public ModelHawkesFixedSumExpKernCustom {

public:
    /**
     * @brief Constructor
     * \param decay : decay for this model (remember that decay is fixed!)
     * \param n_threads : number of threads that will be used for parallel computations
     */
    ModelHawkesSumExpCustom(const ArrayDouble _decays, const ulong _MaxN_of_f, const int max_n_threads = 1);

    using ModelHawkesSingle::set_data;
    void set_data(const SArrayDoublePtrList1D &_timestamps,
                  const SArrayLongPtr _global_n,
                  const double _end_times);

private:
    void allocate_weights() override;

    /**
     * @brief Precomputations of intermediate values for component i
     * \param i : selected component
     */
    void compute_weights_dim_i(const ulong i);//! override;

    /**
     * @brief Return the start of alpha i coefficients in a coeffs vector
     * @param i : selected dimension
     */
    ulong get_alpha_i_first_index(const ulong u, const ulong i) const override {
     return n_nodes + u * n_nodes * n_nodes + i * n_nodes;
    }

    /**
     * @brief Return the end of alpha i coefficients in a coeffs vector
     * @param i : selected dimension
     */
    ulong get_alpha_i_last_index(const ulong u, const ulong i) const override {
     return n_nodes + u * n_nodes * n_nodes + (i + 1) * n_nodes;
    }

    ulong get_alpha_u_i_j_index(const ulong u, const ulong i, const ulong j) const{
        return n_nodes + u * n_nodes * n_nodes + i * n_nodes + j;
    }


    //! The following part is hacked
    //! For hawkes_custom
    /**
     * @brief Return the start of f_i(n) in a coeffs vector
     * @param i : selected dimension
     */
    ulong get_f_i_first_index(const ulong i) const override {
     return n_nodes + n_nodes * n_nodes * decays.size() + i * MaxN_of_f;
    }

    /**
     * @brief Return the end of f_i(n) in a coeffs vector
     * @param i : selected dimension
     */
    ulong get_f_i_last_index(const ulong i) const override {
     return n_nodes + n_nodes * n_nodes * decays.size() + (i + 1) * MaxN_of_f;
    }

public:
    ulong get_n_coeffs() const override;

    ulong get_n_decays() const {
        return decays.size();
    }

    //! @brief Returns decay that was set
    SArrayDoublePtr get_decays() const {
        ArrayDouble copied_decays = decays;
        return copied_decays.as_sarray_ptr();
    }

    /**
     * @brief Set new decay
     * \param decay : new decay
     * \note Weights will need to be recomputed
     */
    void set_decays(ArrayDouble decays) {
     this->decays = decays;
     weights_computed = false;
    }

//    friend ModelHawkesCustomList;
};

#endif  // TICK_OPTIM_MODEL_SRC_CUSTOM_SUMEXP