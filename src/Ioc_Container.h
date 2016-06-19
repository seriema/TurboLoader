#ifndef A_RETRO_UI_IOC_CONTAINER_H
#define A_RETRO_UI_IOC_CONTAINER_H

#include <memory>
#include <map>
#include <type_traits>
#include <functional>
#include <iostream>
#include <string>

namespace Ioc
{
	using std::shared_ptr;
	using std::make_shared;

	class Container
	{
		class IHolder
		{
		public:
			virtual void noop(){}
		};

		template< class T >
		class Holder : public IHolder
		{
		public:
			shared_ptr< T > instance_;
		};

		std::map< std::string, std::function<void*()> > creatorMap_;
		std::map< std::string, shared_ptr<IHolder> >    instanceMap_;

		template< class I, class T >
		void create_holder( shared_ptr< T > instance )
		{
			auto holder = make_shared< Holder< I > >();
			holder->instance_ = instance;
			instanceMap_[ typeid( I ).name() ] = holder;
		};

		template < class T >
		shared_ptr< T > resolve_or_crash()
		{
			auto instance = resolve<T>();
			if ( !instance )
			{
				std::cout << "Couln't resolve '" << typeid( T ).name() << "'" << std::endl;
				throw;
			}
			return instance;
		}

	public:

		// Register singleton.

		template < class T >
		void singleton()
		{
			auto instance = make_shared< T >();
			create_holder<T>( instance );
		}

		template < class I, class T, typename ...Ts >
		typename std::enable_if< std::is_base_of<I, T>::value, void>::type
		singleton()
		{
			auto instance = std::make_shared< T >( resolve_or_crash<Ts>()... );
			create_holder<I>( instance );
			create_holder<T>( instance );
		}

		template < class T, typename T1, typename ...Ts >
		typename std::enable_if< !std::is_base_of<T, T1>::value, void>::type
		singleton()
		{
			auto instance = make_shared< T >( resolve_or_crash<T1>(), resolve_or_crash<Ts>()... );
			create_holder<T>( instance );
		}

		template< class T, class ...Ts >
		void singleton( Ts&& ...ts )
		{
			auto instance = make_shared< T >( std::forward< Ts >( ts )... );
			create_holder<T>( instance );
		}

		template< class I, class T, class ...Ts >
		typename std::enable_if< std::is_base_of<I, T>::value, void>::type
		singleton( Ts&& ...ts )
		{
			auto instance = make_shared< T >( std::forward< Ts >( ts )... );
			create_holder<I>( instance );
			create_holder<T>( instance );
		}

		// Register instance as singleton.

		template < class I, class T >
		inline
		typename std::enable_if< std::is_base_of<I, T>::value, void>::type
		instance( std::shared_ptr<T> i )
		{
			create_holder<I>( i );
		}

		template < class T >
		inline void instance( std::shared_ptr<T> i )
		{
			create_holder<T>( i );
		}

		// Register transient.

		template < class T, typename... Ts >
		void transient()
		{
			auto create_fn = [this]() -> T * {
				return new T( resolve_or_crash<Ts>()... );
			};
			creatorMap_[ typeid(T).name() ] = create_fn;
		}

		// Resolve instance.

		template < class T >
		shared_ptr< T > resolve()
		{
			auto iholder = instanceMap_.find( typeid(T).name() );
			if( iholder != instanceMap_.end() )
			{
				auto holder = dynamic_cast< Holder<T>* >( iholder->second.get() );
				return holder->instance_;
			}

			auto create_fn = creatorMap_.find( typeid(T).name() );
			if ( create_fn != creatorMap_.end() )
			{
				return shared_ptr<T>( static_cast<T*>(create_fn->second()) );
			}

			return nullptr;
		}
	};
}

#endif //A_RETRO_UI_IOC_CONTAINER_H
