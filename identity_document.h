#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument {
public:
    IdentityDocument()
        : unique_id_(++unique_id_count_) 
    {
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_) 
    {
        IdentityDocument::SetVTable(this);
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void PrintID() const {
        GetVtable()->print_id(this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }
    
    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

    using PrintIDFunction = void(*)(const IdentityDocument*);
    using DeleteFunction = void(*)(IdentityDocument*);

    struct Vtable {
        PrintIDFunction print_id;
        DeleteFunction delete_this;
    };

    static void SetVTable(IdentityDocument* obj) {
        //(new-type) expression
        *(IdentityDocument::Vtable**)obj = &IdentityDocument::VTABLE;
    }

    const Vtable* GetVtable() const {
        return vptr_;
    }

    Vtable* GetVtable() {
        return vptr_;
    }

    static IdentityDocument::Vtable VTABLE;
    
    int GetID() const {
        return unique_id_;
    }

private:
    IdentityDocument::Vtable* vptr_ = nullptr;
    static int unique_id_count_;
    int unique_id_;

    static void PrintID(const IdentityDocument* obj) {
        std::cout << "IdentityDocument::PrintID() : "sv << obj->unique_id_ << std::endl;
    }

    static void Delete(IdentityDocument* obj) {
        //В этот момент тип объекта известен. Просто удаляем указатель.
        //Вызов delete запустит процесс вызовов деструкторов
        delete obj;
    }
};

IdentityDocument::Vtable IdentityDocument::VTABLE = { IdentityDocument::PrintID, IdentityDocument::Delete };

int IdentityDocument::unique_id_count_ = 0;