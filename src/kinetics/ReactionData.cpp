//! @file ReactionData.cpp

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#include "cantera/kinetics/ReactionData.h"
#include "cantera/kinetics/Kinetics.h"
#include "cantera/thermo/ThermoPhase.h"
#include "cantera/base/ctexceptions.h"

namespace Cantera
{

std::pair<bool, bool> ArrheniusData::update(const ThermoPhase& bulk,
                                            const Kinetics& kin)
{
    double T = bulk.temperature();
    std::pair<bool, bool> changed{ false, T != temperature };
    update(T);
    return changed;
}

void BlowersMaselData::update(double T)
{
    temperature = T;
    logT = std::log(T);
    recipT = 1./T;
}

BlowersMaselData::BlowersMaselData()
    : temperature(1.)
    , logT(0.)
    , recipT(1.)
    , density(NAN)
    , state_mf_number(-1)
    , finalized(false)
{
}

std::pair<bool, bool> BlowersMaselData::update(const ThermoPhase& bulk,
                                               const Kinetics& kin)
{
    double rho = bulk.density();
    int mf = bulk.stateMFNumber();
    double T = bulk.temperature();
    std::pair<bool, bool> changed { false, T != temperature };
    if (T != temperature || rho != density || mf != state_mf_number) {
        density = rho;
        state_mf_number = mf;
        bulk.getPartialMolarEnthalpies(m_grt.data());
        kin.getReactionDelta(m_grt.data(), dH.data());
        changed.first = changed.second = true;
    }
    update(T);
    return changed;
}

std::pair<bool, bool> FalloffData::update(const ThermoPhase& bulk, const Kinetics& kin)
{
    double rho_m = bulk.molarDensity();
    int mf = bulk.stateMFNumber();
    double T = bulk.temperature();
    std::pair<bool, bool> changed { false, T != temperature };
    if (rho_m != molar_density || mf != state_mf_number) {
        molar_density = rho_m;
        state_mf_number = mf;
        kin.getThirdBodyConcentrations(conc_3b.data());
        changed.first = changed.second = true;
    }
    update(T);
    return changed;
}

void PlogData::update(double T)
{
    throw CanteraError("PlogData::update",
        "Missing state information: reaction type requires pressure.");
}

std::pair<bool, bool> PlogData::update(const ThermoPhase& bulk, const Kinetics& kin)
{
    double T = bulk.temperature();
    double P = bulk.pressure();
    std::pair<bool, bool> changed{ P != pressure, P != pressure || T != temperature };
    update(T, P);
    return changed;
}

void ChebyshevData::update(double T)
{
    throw CanteraError("ChebyshevData::update",
        "Missing state information: reaction type requires pressure.");
}

std::pair<bool, bool> ChebyshevData::update(const ThermoPhase& bulk, const Kinetics& kin)
{
    double T = bulk.temperature();
    double P = bulk.pressure();
    std::pair<bool, bool> changed{ P != pressure, P != pressure || T != temperature };
    update(T, P);
    return changed;
}

std::pair<bool, bool> CustomFunc1Data::update(const ThermoPhase& bulk, const Kinetics& kin)
{
    double T = bulk.temperature();
    std::pair<bool, bool> changed { false, T != temperature };
    temperature = T;
    return changed;
}

}
