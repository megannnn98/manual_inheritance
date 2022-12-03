#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence {
public:
    DrivingLicence() 
    : identityDocument_()
    {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : identityDocument_(other.identityDocument_)
    {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }

    int GetID() const {
        return identityDocument_.GetID();
    }

    void PrintID() const {
        GetVtable()->print_id(this);
    }
    
    void Delete() {
        GetVtable()->delete_this(this);
    }

    operator IdentityDocument() {
        return {identityDocument_};
    }

    using PrintIDFunction = void(*)(const DrivingLicence*);
    using DeleteFunction = void(*)(DrivingLicence*);

    struct Vtable {
        PrintIDFunction print_id;
        DeleteFunction delete_this;
    };

    static void SetVTable(DrivingLicence* obj) {
        *(DrivingLicence::Vtable**)obj = &DrivingLicence::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const DrivingLicence::Vtable*)identityDocument_.GetVtable();
    }

    Vtable* GetVtable() {
        return (DrivingLicence::Vtable*)identityDocument_.GetVtable();
    }

    static DrivingLicence::Vtable VTABLE;

private:
    IdentityDocument identityDocument_;

    static void PrintID(const DrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->identityDocument_.GetID() << std::endl;
    }
 
    static void Delete(DrivingLicence* obj) {
        //В этот момент тип объекта известен. Просто удаляем указатель.
        //Вызов delete запустит процесс вызовов деструкторов
        delete obj;
    }
};

DrivingLicence::Vtable DrivingLicence::VTABLE = { DrivingLicence::PrintID, DrivingLicence::Delete };
