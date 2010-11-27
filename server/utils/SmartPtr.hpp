
#ifndef __SMARTPTR_HPP__
# define __SMARTPTR_HPP__

# include "thread/ScopeLock.hpp"

# include "Atomic.hpp"

namespace zhttpd
{
    template<typename PtrType>
    class SmartPtr
    {
    private:
        int* _count;
        PtrType* _ptr;

        void _destroyPtr()
        {
            if (Atomic::decrement(this->_count) <= 0)
            {
                delete this->_count;
                delete this->_ptr;
            }
        }

    public:
        SmartPtr(PtrType* ptr = 0)
        {
            this->_count = new int;
            *this->_count = 1;
            this->_ptr = ptr;
        }

        SmartPtr(SmartPtr<PtrType> const& ref)
        {
            this->_count = ref._count;
            this->_ptr = ref._ptr;
            Atomic::increment(this->_count);
        }

        template<typename PtrTypeChild>
        SmartPtr(SmartPtr<PtrTypeChild> const& ref)
        {
            this->_count = ref.getCountPtr();
            this->_ptr = ref.get();
            Atomic::increment(this->_count);
        }

        ~SmartPtr()
        {
            this->_destroyPtr();
        }

        SmartPtr<PtrType>& operator =(PtrType* ptr)
        {
            this->_destroyPtr();
            this->_count = new int;
            *this->_count = 1;
            this->_ptr = ptr;
            return *this;
        }

        SmartPtr<PtrType>& operator =(SmartPtr<PtrType> const& ref)
        {
            if (this != &ref)
            {
                this->_destroyPtr();
                this->_count = ref._count;
                this->_ptr = ref._ptr;
                Atomic::increment(this->_count);
            }
            return (*this);
        }

        PtrType& operator *() const
        {
            return *this->_ptr;
        }

        PtrType* operator ->() const
        {
            return this->_ptr;
        }

        int getCount() const
        {
            return *this->_count;
        }

        PtrType* get() const
        {
            return this->_ptr;
        }

        int* getCountPtr() const
        {
            return this->_count;
        }
    };
}

#endif // !__SMARTPTR_HPP__
