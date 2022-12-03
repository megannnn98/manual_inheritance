#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std::string_view_literals;

class Passport {
public:
    Passport()
        : identityDocument_(), 
          expiration_date_(GetExpirationDate()) 
    {
        Passport::SetVTable(this);
        std::cout << "Passport::Ctor()"sv << std::endl;
    }

    Passport(const Passport& other)
        : identityDocument_(other.identityDocument_)
        , expiration_date_(other.expiration_date_) 
    {
        Passport::SetVTable(this);
        std::cout << "Passport::CCtor()"sv << std::endl;
    }

    ~Passport() {
        std::cout << "Passport::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }

    static void PrintUniqueIDCount() {
        IdentityDocument::PrintUniqueIDCount();
    }

    void PrintID() const {
        GetVtable()->print_id(this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    void PrintVisa(const std::string& country) const {
        GetVtable()->print_visa(this, country);
    }
    
    operator IdentityDocument() {
        return {identityDocument_};
    }

    using PrintIDFunction = void(*)(const Passport*);
    using DeleteFunction = void(*)(Passport*);
    using PrintVisaFunction = void(*)(const Passport*, const std::string&);

    struct Vtable {
        PrintIDFunction print_id;
        DeleteFunction delete_this;
        PrintVisaFunction print_visa;
    };

    static void SetVTable(Passport* obj) {
        *(Passport::Vtable**)obj = &Passport::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const Passport::Vtable*)identityDocument_.GetVtable();
    }

    Vtable* GetVtable() {
        return (Passport::Vtable*)identityDocument_.GetVtable();
    }

    static Passport::Vtable VTABLE;

    int GetID() const {
        return identityDocument_.GetID();
    }

private:
    IdentityDocument identityDocument_;
    const struct tm expiration_date_;

    tm GetExpirationDate() {
	    time_t t = time(nullptr);
	    tm exp_date = *localtime(&t);
	    exp_date.tm_year += 10;
	    mktime(&exp_date);
	    return exp_date;
	}

    static void PrintID(const Passport* obj) {
        std::cout << "Passport::PrintID() : "sv << obj->GetID();
        std::cout << " expiration date : "sv << obj->expiration_date_.tm_mday << "/"sv << obj->expiration_date_.tm_mon << "/"sv
                  << obj->expiration_date_.tm_year + 1900 << std::endl;
    }

    static void PrintVisa(const Passport* obj, const std::string& country) {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << obj->identityDocument_.GetID() << std::endl;
    }

    static void Delete(Passport* obj) {
        //В этот момент тип объекта известен. Просто удаляем указатель.
        //Вызов delete запустит процесс вызовов деструкторов
        delete obj;
    }
};

Passport::Vtable Passport::VTABLE = { Passport::PrintID, Passport::Delete, Passport::PrintVisa };
