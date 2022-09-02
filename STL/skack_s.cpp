#include<exception>//std::exception
#include<memory>//std::share_ptr
#include<mutex>//std::mutex
#include<stack>//std::stack
struct empty_shack:std::exception
{
    /* data */
    const char* what() const throw();
};
template <typename T>
class threadsafe_shack
{
private:
    std::stack<T>data;
    mutable std::mutex m;
public:
    threadsafe_shack(){};
    threadsafe_shack(const threadsafe_shack& other)
    {
        std::lock_guard<std::mutex>lock(m);
        data=other.data;
    }
    threadsafe_shack& operator=(const threadsafe_shack& )=delete;
    void push(T new_value){
        std::lock_guard<std::mutex>lock(m);
        data.push(std::move(new_value));
    }
    std::shared_ptr<T>pop()
    {
        std::lock_guard<std::mutex>lock(m);
        if(data.empty())throw empty_shack;
        std::shared_ptr<T> const res(std::make_shared<T>(data.top));
        data.pop();
        return res;
    }
    void pop(T&value)
    {
        std::lock_guard<std::mutex>lock(m);
        if(data.empty())throw empty_shack;
        value=data.top();
        data.pop();
        
    }
    bool empty()const{
        std::lock_guard<std::mutex>lock(m);
        return data.empty();
    }
};


