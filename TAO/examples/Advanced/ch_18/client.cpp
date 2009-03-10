// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/examples/Advanced/ch_18
//
// = FILENAME
//    client.cpp
//
// = AUTHORS
//   Source code used in TAO has been modified and adapted from the code
//   provided in the book, "Advanced CORBA Programming with C++" by Michi
//   Henning and Steve Vinoski. Copyright 1999. Addison-Wesley, Reading,
//   MA.
//
//   Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
//
// ============================================================================

#include "CCSC.h"        // ORB-specific
#include "assert.h"
#include <orbsvcs/CosNamingC.h>
#include <ace/streams.h>
// ----------------------------------------------------------------

using namespace std;

template<class T>
typename T::_ptr_type
resolve_init(CORBA::ORB_ptr orb, const char * id)
{
    CORBA::Object_var obj;
    try {
        obj = orb->resolve_initial_references(id);
    }
    catch (const CORBA::ORB::InvalidName &) {
        throw;
    }
    catch (const CORBA::Exception & e) {
        std::cerr << "Cannot get initial reference for "
             << id << ": "
             << e
             << std::endl;
        throw 0;
    }
    assert(!CORBA::is_nil(obj.in()));

    typename T::_var_type ref;
    try {
        ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
        std::cerr << "Cannot narrow reference for "
             << id << ": "
             << e
             << std::endl;
        throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
        std::cerr << "Incorrect type of reference for "
             << id << std::endl;
        throw 0;
    }
    return ref._retn();
}

//----------------------------------------------------------------

template<class T>
typename T::_ptr_type
resolve_name(
    CosNaming::NamingContext_ptr    nc,
    const CosNaming::Name &         name)
{
    CORBA::Object_var obj;
    try {
        obj = nc->resolve(name);
    }
    catch (const CosNaming::NamingContext::NotFound &) {
        throw;
    }
    catch (const CORBA::Exception & e) {
        std::cerr << "Cannot resolve binding: "
                  << e
                  << std::endl;
        throw 0;
    }
    if (CORBA::is_nil(obj.in())) {
        std::cerr << "Nil binding in Naming Service" << std::endl;
        throw 0;
    }

    typename T::_var_type ref;
    try {
        ref = T::_narrow(obj.in());
    }
    catch (const CORBA::Exception & e) {
        std::cerr << "Cannot narrow reference: "
                  << e
                  << std::endl;
        throw 0;
    }
    if (CORBA::is_nil(ref.in())) {
        std::cerr << "Reference has incorrect type" << std::endl;
        throw 0;
    }
    return ref._retn();
}

//----------------------------------------------------------------

// Generic ostream inserter for exceptions. Inserts the exception
// name, if available, and the repository ID otherwise.

#if 0   // This inserter is not be needed for TAO.

static ostream &
operator<<(ostream & os, const CORBA::Exception & e)
{
    CORBA::Any tmp;
    tmp <<= e;

    CORBA::TypeCode_var tc = tmp.type();
    const char * p = tc->name();
    if (*p != '\0')
        os << p;
    else
        os << tc->id();
    return os;
}

#endif

//----------------------------------------------------------------

// Show the details for a thermometer or thermostat.

static std::ostream &
operator<<(std::ostream &os, CCS::Thermometer_ptr t)
{
    // Check for nil.
    if (CORBA::is_nil(t)) {
        os << "Cannot show state for nil reference." << std::endl;
        return os;
    }

    // Try to narrow and print what kind of device it is.
    CCS::Thermostat_var tmstat = CCS::Thermostat::_narrow(t);
    os << (CORBA::is_nil(tmstat.in()) ? "Thermometer:" : "Thermostat:")
       << std::endl;

    // Show attribute values.
    CCS::ModelType_var model = t->model();
    CCS::LocType_var location = t->location();
    os << "\tAsset number: " << t->asset_num() << std::endl;
    os << "\tModel       : " << model.in() << std::endl;
    os << "\tLocation    : " << location.in() << std::endl;
    os << "\tTemperature : " << t->temperature() << std::endl;

    // If device is a thermostat, show nominal temperature.
    if (!CORBA::is_nil(tmstat.in()))
        os << "\tNominal temp: " << tmstat->get_nominal() << std::endl;
    return os;
}

//----------------------------------------------------------------

// Show the information in a BtData struct.

static std::ostream &
operator<<(std::ostream &os, const CCS::Thermostat::BtData &btd)
{
    os << "CCS::Thermostat::BtData details:" << std::endl;
    os << "\trequested    : " << btd.requested << std::endl;
    os << "\tmin_permitted: " << btd.min_permitted << std::endl;
    os << "\tmax_permitted: " << btd.max_permitted << std::endl;
    os << "\terror_msg    : " << btd.error_msg << std::endl;
    return os;
}

//----------------------------------------------------------------

// Loop over the sequence of records in an EChange exception and
// show the details of each record.

static std::ostream &
operator<<(std::ostream &os, const CCS::Controller::EChange &ec)
{
    for (CORBA::ULong i = 0; i < ec.errors.length(); i++) {
        os << "Change failed:" << std::endl;
        os << ec.errors[i].tmstat_ref.in(); // Overloaded <<
        os << ec.errors[i].info << std::endl;    // Overloaded <<
    }
    return os;
}

//----------------------------------------------------------------

// Change the temperature of a thermostat.

static void
set_temp(CCS::Thermostat_ptr tmstat, CCS::TempType new_temp)
{
    if (CORBA::is_nil(tmstat))  // Don't call via nil reference
        return;

    CCS::AssetType anum = tmstat->asset_num();
    try {
        std::cout << "Setting thermostat " << anum
             << " to " << new_temp << " degrees." << std::endl;
        CCS::TempType old_nominal = tmstat->set_nominal(new_temp);
        std::cout << "Old nominal temperature was: "
             << old_nominal << std::endl;
        std::cout << "New nominal temperature is: "
             << tmstat->get_nominal() << std::endl;
    } catch (const CCS::Thermostat::BadTemp &bt) {
        std::cerr << "Setting of nominal temperature failed." << std::endl;
        std::cerr << bt.details << std::endl;             // Overloaded <<
    }
}

//----------------------------------------------------------------

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
    CORBA::ULong i = 0;

    try {
        // Initialize the ORB
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        // Check arguments
        if (argc != 1) {
            std::cerr << "Usage: client" << std::endl;
            throw 0;
        }

        // Get reference to initial naming context.
        CosNaming::NamingContext_var inc
            = resolve_init<CosNaming::NamingContext>(
                    orb.in(), "NameService"
              );

        // Look for controller in the Naming Service.
        CosNaming::Name n;
        n.length(2);
        n[0].id = CORBA::string_dup("CCS");
        n[1].id = CORBA::string_dup("Controller");
        CCS::Controller_var ctrl;
        try {
            ctrl = resolve_name<CCS::Controller>(inc.in(), n);
        } catch (const CosNaming::NamingContext::NotFound &) {
            std::cerr << "No controller in Naming Service" << std::endl;
            throw 0;
        }

        // Get list of devices
        CCS::Controller::ThermometerSeq_var list = ctrl->list();

        // Show number of devices.
        CORBA::ULong len = list->length();
        std::cout << "Controller has " << len << " device";
        if (len != 1)
            std::cout << "s";
        std::cout << "." << std::endl;

        CCS::Thermometer_var t = ctrl->create_thermometer(27, "Room 1");
        CCS::Thermostat_var ts = ctrl->create_thermostat(28, "Room 2", 48);
        CCS::Thermostat_var ts2 = ctrl->create_thermostat(30, "Room 3", 48);
        CCS::Thermostat_var ts3 = ctrl->create_thermostat(32, "Room 3", 68);
        CCS::Thermostat_var ts4 = ctrl->create_thermostat(34, "Room 3", 68);
        CCS::Thermostat_var ts5 = ctrl->create_thermostat(36, "Room 3", 48);
        std::cout << t->location() << std::endl;
        std::cout << ts->location() << std::endl;
        std::cout << ts2->location() << std::endl;
        t->remove();

        list = ctrl->list();
        // Show details for each device.
        for ( i = 0; i < list->length(); i++)
            std::cout << list[i];
        std::cout << std::endl;

        // Change the location of first device in the list
        CCS::AssetType anum = list[0u]->asset_num();
        std::cout << "Changing location of device "
             << anum << "." << std::endl;
        list[0u]->location("Earth");
        // Check that the location was updated
        std::cout << "New details for device "
             << anum << " are:" << std::endl;
        std::cout << list[0u] << std::endl;

        // Find first thermostat in list.
        CCS::Thermostat_var tmstat;
        for (   i = 0;
                i < list->length() && CORBA::is_nil(tmstat.in());
                i++) {
            tmstat = CCS::Thermostat::_narrow(list[i]);
        }

        // Check that we found a thermostat on the list.
        if (CORBA::is_nil(tmstat.in())) {
            std::cout << "No thermostat devices in list." << std::endl;
        } else {
            // Set temperature of thermostat to
            // 50 degrees (should work).
            set_temp(tmstat.inout(), 50);
            std::cout << std::endl;

            // Set temperature of thermostat to
            // -10 degrees (should fail).
            set_temp(tmstat.inout(), -10);
        }

        // Look for device in Rooms Earth and HAL. This must
        // locate at least one device because we earlier changed
        // the location of the first device to Room Earth.
        std::cout << "Looking for devices in Earth and HAL." << std::endl;
        CCS::Controller::SearchSeq ss;
        ss.length(2);
        ss[0].key.loc(CORBA::string_dup("Earth"));
        ss[1].key.loc(CORBA::string_dup("HAL"));
        ctrl->find(ss);

        // Show the devices found in that room.
        for ( i = 0; i < ss.length(); i++)
            std::cout << ss[i].device.in();          // Overloaded <<
        std::cout << std::endl;

        // Increase the temperature of all thermostats
        // by 40 degrees. First, make a new list (tss)
        // containing only thermostats.
        std::cout << "Increasing thermostats by 40 degrees." << std::endl;
        CCS::Controller::ThermostatSeq tss;
        for ( i = 0; i < list->length(); i++) {
            tmstat = CCS::Thermostat::_narrow(list[i]);
            if (CORBA::is_nil(tmstat.in()))
                continue;                   // Skip thermometers
            len = tss.length();
            tss.length(len + 1);
            tss[len] = tmstat;
        }

        // Try to change all thermostats.
        try {
            ctrl->change(tss, 40);
        } catch (const CCS::Controller::EChange &ec) {
            std::cerr << ec;                     // Overloaded <<
        }
    } catch (const CORBA::Exception & e) {
        std::cerr << "Uncaught CORBA exception: "
                  << e
                  << std::endl;
        return 1;
    } catch (...) {
        return 1;
    }
    return 0;
}
