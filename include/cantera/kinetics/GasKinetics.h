/**
 * @file GasKinetics.h
 * @ingroup chemkinetics
 */

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_GASKINETICS_H
#define CT_GASKINETICS_H

#include "BulkKinetics.h"
#include "FalloffMgr.h"
#include "Reaction.h"

namespace Cantera
{

/**
 * Kinetics manager for elementary gas-phase chemistry. This kinetics manager
 * implements standard mass-action reaction rate expressions for low-density
 * gases.
 * @ingroup kinetics
 */
class GasKinetics : public BulkKinetics
{
public:
    //! @name Constructors and General Information
    //! @{

    //! Constructor.
    /*!
     *  @param thermo  Pointer to the gas ThermoPhase (optional)
     */
    GasKinetics(ThermoPhase* thermo = 0);

    virtual std::string kineticsType() const {
        return "Gas";
    }

    virtual void getThirdBodyConcentrations(double* concm) const;

    //! @}
    //! @name Reaction Rates Of Progress
    //! @{

    virtual void getEquilibriumConstants(doublereal* kc);
    virtual void getFwdRateConstants(double* kfwd);

    //! @}
    //! @name Reaction Mechanism Setup Routines
    //! @{
    virtual bool addReaction(shared_ptr<Reaction> r, bool resize=true);
    virtual void modifyReaction(size_t i, shared_ptr<Reaction> rNew);
    virtual void invalidateCache();
    //! @}

    void updateROP();

    //! Update temperature-dependent portions of reaction rates and falloff
    //! functions.
    virtual void update_rates_T();

    //! Update properties that depend on concentrations.
    //! Currently the enhanced collision partner concentrations are updated
    //! here, as well as the pressure-dependent portion of P-log and Chebyshev
    //! reactions.
    virtual void update_rates_C();

protected:
    //! Reaction index of each falloff reaction
    std::vector<size_t> m_fallindx; //!< @deprecated (legacy only)

    //! Map of reaction index to falloff reaction index (i.e indices in
    //! #m_falloff_low_rates and #m_falloff_high_rates)
    std::map<size_t, size_t> m_rfallindx; //!< @deprecated (legacy only)

    //! Rate expressions for falloff reactions at the low-pressure limit
    Rate1<Arrhenius> m_falloff_low_rates; //!< @deprecated (legacy only)

    //! Rate expressions for falloff reactions at the high-pressure limit
    Rate1<Arrhenius> m_falloff_high_rates; //!< @deprecated (legacy only)

    FalloffMgr m_falloffn; //!< @deprecated (legacy only)

    ThirdBodyCalc m_3b_concm; //!< @deprecated (legacy only)
    ThirdBodyCalc m_falloff_concm; //!< @deprecated (legacy only)

    Rate1<Plog> m_plog_rates; //!< @deprecated (legacy only)
    Rate1<Chebyshev> m_cheb_rates; //!< @deprecated (legacy only)

    //! @name Reaction rate data
    //!@{
    doublereal m_logStandConc;
    vector_fp m_rfn_low; //!< @deprecated (legacy only)
    vector_fp m_rfn_high; //!< @deprecated (legacy only)

    doublereal m_pres; //!< Last pressure at which rates were evaluated
    vector_fp falloff_work; //!< @deprecated (legacy only)
    vector_fp concm_3b_values; //!< @deprecated (legacy only)
    vector_fp concm_falloff_values; //!< @deprecated (legacy only)

    //!@}

    // functions marked as deprecated below are only used for XML import and
    // transitional reaction types that are marked as '-legacy'

    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void processFalloffReactions();

    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void addThreeBodyReaction(ThreeBodyReaction2& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void addFalloffReaction(FalloffReaction& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void addPlogReaction(PlogReaction2& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void addChebyshevReaction(ChebyshevReaction2& r);

    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void modifyThreeBodyReaction(size_t i, ThreeBodyReaction2& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void modifyFalloffReaction(size_t i, FalloffReaction& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void modifyPlogReaction(size_t i, PlogReaction2& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void modifyChebyshevReaction(size_t i, ChebyshevReaction2& r);

    //! Update the equilibrium constants in molar units.
    void updateKc();
};

}

#endif
