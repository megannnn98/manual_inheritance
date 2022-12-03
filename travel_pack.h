#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class TravelPack {
public:
    TravelPack()
        : identityDocument_()
        , identity_doc1_((IdentityDocument*)new Passport())
        , identity_doc2_((IdentityDocument*)new DrivingLicence()) 
    {
        TravelPack::SetVTable(this);
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
        : identityDocument_(other.identityDocument_) 
        , identity_doc1_((IdentityDocument*)new Passport(*(Passport*)(other.identity_doc1_)))
        , identity_doc2_((IdentityDocument*)new DrivingLicence(*(DrivingLicence*)(other.identity_doc2_)))
        , additional_pass_(other.additional_pass_)
        , additional_dr_licence_(other.additional_dr_licence_) 
    {
        TravelPack::SetVTable(this);
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack() {
        identity_doc1_->Delete();
        identity_doc2_->Delete();
        std::cout << "TravelPack::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
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

    using PrintIDFunction = void(*)(const TravelPack*);
    using DeleteFunction = void(*)(TravelPack*);

    struct Vtable {
        PrintIDFunction print_id;
        DeleteFunction delete_this;
    };

    static void SetVTable(TravelPack* obj) {
        *(TravelPack::Vtable**)obj = &TravelPack::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const TravelPack::Vtable*)identityDocument_.GetVtable();
    }

    Vtable* GetVtable() {
        return (TravelPack::Vtable*)identityDocument_.GetVtable();
    }

    static TravelPack::Vtable VTABLE;

private:
    IdentityDocument identityDocument_;
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;

    static void PrintID(const TravelPack* obj) {
        obj->identity_doc1_->PrintID();
        obj->identity_doc2_->PrintID();
        obj->additional_pass_.PrintID();
        obj->additional_dr_licence_.PrintID();
    }
 
    static void Delete(TravelPack* obj) {
        //В этот момент тип объекта известен. Просто удаляем указатель.
        //Вызов delete запустит процесс вызовов деструкторов
        delete obj;
    }
};

TravelPack::Vtable TravelPack::VTABLE = { TravelPack::PrintID, TravelPack::Delete };
