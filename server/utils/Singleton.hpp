
#ifndef __SINGLETON_HPP__
# define __SINGLETON_HPP__

#include <cassert>

namespace ZHTTPD
{

  /** Classe commune pour déclarer des Singleton
   * @example common/Singleton_example.cpp
   * La manière de déclarer une classe en tant que singleton
   */
  template <typename T>
    class Singleton
    {
      private:
        static T* _instance;
        explicit Singleton(Singleton const&) {}
        Singleton& operator =(Singleton const&) { return *this; }

      protected:
        explicit Singleton() {
          assert(Singleton<T>::_instance == 0);
        }
        virtual ~Singleton() {}
      public:
        /// Récupère l'instance du Singleton
        static T* getInstance(void)
        {
          if (Singleton<T>::_instance == 0)
            Singleton<T>::_instance = new T;
          return Singleton<T>::_instance;
        }
        /// Supprime l'instance du Singleton
        static void delInstance(void)
        {
          if (Singleton<T>::_instance != 0)
            delete Singleton<T>::_instance;
          Singleton<T>::_instance = 0;
        }
    };

  template<typename T> T* Singleton<T>::_instance = 0;
}

#endif /* !__SINGLETON_HPP__ */

