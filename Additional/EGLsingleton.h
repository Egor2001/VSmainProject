#pragma once

#include "../stdafx.h"

template<class S>
class EGLsingleton
{
    static_assert(!std::is_constructible<S>::value, "Singleton mustn't have public constructors ");
    static_assert(!std::is_assignable   <S>::value, "Singleton mustn't have public operator = ");

public:
    typedef S Singleton_;

    template<typename ...Types>
    static Singleton_* create(Types&& ...args)
    {
        if (instance_) throw std::runtime_error(std::string("instance of singleton class ") + std::string(typeid(Singleton_).name()) +
                                                std::string(" has been created before calling 'create()' function"));

        return (instance_ = std::unique_ptr<Singleton_>(new Singleton_(std::forward<Types>(args)...))).get();
    }

    static void destroy() { instance_.reset(); }

    static Singleton_* instance()
    {
        if (!instance_) throw std::runtime_error(std::string("instance of singleton class ") + std::string(typeid(Singleton_).name()) +
                                                 std::string(" has not been created before calling 'instance()' function"));

        return instance_.get();
    }

protected:
    EGLsingleton         () = default;
    virtual ~EGLsingleton() = default;

private:
    EGLsingleton(const EGLsingleton&) = delete;
    EGLsingleton(EGLsingleton&&)      = delete;

    EGLsingleton& operator = (const EGLsingleton&) = delete;
    EGLsingleton& operator = (EGLsingleton&&)      = delete;

    static std::unique_ptr<Singleton_> instance_;
};

template<class S> std::unique_ptr<S> EGLsingleton<S>::instance_;