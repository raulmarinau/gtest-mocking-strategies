#pragma once

template<typename F>
class MyBetterDesignedTemplateClass
{
public:
    MyBetterDesignedTemplateClass()
    {
        myFoo = std::make_shared<F>();
    }

    void doStuff()
    {
        if (myFoo->useFoo("Ana", 3)) std::cout << "Ana are destule mere\n";
        else std::cout << "Nu sunt destule mere\n";
    }

    std::shared_ptr<F> getFoo()
    {
        return myFoo;
    }

private:
    std::shared_ptr<F> myFoo;
};
