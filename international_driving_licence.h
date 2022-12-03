#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence
{
public:
    InternationalDrivingLicence()
        : drivingLicence_()
    {
        InternationalDrivingLicence::SetVTable(this);
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence &other)
        : drivingLicence_(other.drivingLicence_)
    {
        InternationalDrivingLicence::SetVTable(this);
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence()
    {
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
        DrivingLicence::SetVTable((DrivingLicence *)this);
    }

    void PrintID() const
    {
        GetVtable()->print_id(this);
    }

    void Delete()
    {
        GetVtable()->delete_this(this);
    }

    operator DrivingLicence()
    {
        return {drivingLicence_};
    }

    using PrintIDFunction = void(*)(const InternationalDrivingLicence*);
    using DeleteFunction = void(*)(InternationalDrivingLicence*);

    struct Vtable
    {
        PrintIDFunction print_id;
        DeleteFunction delete_this;
    };

    static void SetVTable(InternationalDrivingLicence *obj)
    {
        *(InternationalDrivingLicence::Vtable **)obj = &InternationalDrivingLicence::VTABLE;
    }

    const Vtable *GetVtable() const
    {
        return (const InternationalDrivingLicence::Vtable *)drivingLicence_.GetVtable();
    }

    Vtable *GetVtable()
    {
        return (InternationalDrivingLicence::Vtable *)drivingLicence_.GetVtable();
    }

    static InternationalDrivingLicence::Vtable VTABLE;

private:
    DrivingLicence drivingLicence_;

    static void PrintID(const InternationalDrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->drivingLicence_.GetID() << std::endl;
    }

    static void Delete(InternationalDrivingLicence *obj)
    {
        //В этот момент тип объекта известен. Просто удаляем указатель.
        //Вызов delete запустит процесс вызовов деструкторов
        delete obj;
    }
};

InternationalDrivingLicence::Vtable InternationalDrivingLicence::VTABLE = {InternationalDrivingLicence::PrintID, InternationalDrivingLicence::Delete};
