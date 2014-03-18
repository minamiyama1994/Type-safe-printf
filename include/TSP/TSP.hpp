#ifndef TSP_HPP
#define TSP_HPP
#include<cstdlib>
#include<type_traits>
#include"sprout/string.hpp"
#include"sprout/type/string.hpp"
#include"FTMP/list.hpp"
#include"FTMP/integral.hpp"
#include"FTMP/make_integer_sequence.hpp"
#include"FTMP/if_c.hpp"
namespace tsp
{
	template < typename >
	class lambda ;
	template < >
	class lambda < char >
	{
		char ch_ ;
	public :
		constexpr lambda ( char ch )
			: ch_ ( ch )
		{
		}
		constexpr lambda ( const lambda & ) = default ;
		constexpr lambda ( lambda && ) = default ;
		auto operator = ( const lambda & ) -> lambda & = delete ;
		auto operator = ( lambda && ) -> lambda & = delete ;
		auto operator ( ) ( ) -> void
		{
			std::cout.put ( ch_ ) ;
		}
	} ;
	template < std::size_t N >
	class lambda < char [ N ] >
	{
		char s_ [ N ] ;
	public :
		template < std::size_t ... Ns >
		constexpr lambda ( ftmp::list < ftmp::integral < std::size_t , Ns > ... > , const char ( & s ) [ sizeof ... ( Ns ) ] )
			: s_ { s [ Ns ] ... }
		{
		}
		constexpr lambda ( const char ( & s ) [ N ] )
			: lambda ( typename ftmp::make_integer_sequence < ftmp::integral < std::size_t , 0 > , ftmp::integral < std::size_t , N > >::type { } , s )
		{
		}
		constexpr lambda ( const lambda & ) = default ;
		constexpr lambda ( lambda && ) = default ;
		auto operator = ( const lambda & ) -> lambda & = delete ;
		auto operator = ( lambda && ) -> lambda & = delete ;
		auto operator ( ) ( ) -> void
		{
			std::cout.write ( s_ , N - 1 ) ;
		}
	} ;
	template < >
	class lambda < bool >
	{
		bool b_ ;
	public :
		constexpr lambda ( bool b )
			: b_ ( b )
		{
		}
		constexpr lambda ( const lambda & ) = default ;
		constexpr lambda ( lambda && ) = default ;
		auto operator = ( const lambda & ) -> lambda & = delete ;
		auto operator = ( lambda && ) -> lambda & = delete ;
		auto operator ( ) ( ) -> void
		{
			std::cout << std::boolalpha << b_ ;
		}
	} ;
	template < >
	class lambda < int >
	{
		int n_ ;
	public :
		constexpr lambda ( int n )
			: n_ ( n )
		{
		}
		constexpr lambda ( const lambda & ) = default ;
		constexpr lambda ( lambda && ) = default ;
		auto operator = ( const lambda & ) -> lambda & = delete ;
		auto operator = ( lambda && ) -> lambda & = delete ;
		auto operator ( ) ( ) -> void
		{
			std::cout << n_ ;
		}
	} ;
	template < >
	class lambda < double >
	{
		double f_ ;
	public :
		constexpr lambda ( double f )
			: f_ ( f )
		{
		}
		constexpr lambda ( const lambda & ) = default ;
		constexpr lambda ( lambda && ) = default ;
		auto operator = ( const lambda & ) -> lambda & = delete ;
		auto operator = ( lambda && ) -> lambda & = delete ;
		auto operator ( ) ( ) -> void
		{
			std::cout << f_ ;
		}
	} ;
	template < typename ... >
	class output_manager ;
	template < typename F , typename ... Fs >
	class output_manager < F , Fs ... >
	{
		F f_ ;
		output_manager < Fs ... > fs_ ;
	public :
		constexpr output_manager ( F f , Fs ... fs )
			: f_ ( f )
			, fs_ ( fs ... )
		{
		}
		constexpr output_manager ( F f , output_manager < Fs ... > fs )
			: f_ ( f )
			, fs_ ( fs )
		{
		}
		auto operator ( ) ( )
		{
			f_ ( ) ;
			fs_ ( ) ;
		}
	} ;
	template < typename F >
	class output_manager < F >
	{
		F f_ ;
	public :
		constexpr output_manager ( F f )
			: f_ ( f )
		{
		}
		auto operator ( ) ( )
		{
			f_ ( ) ;
		}
	} ;
	template < >
	class output_manager < >
	{
	public :
		constexpr output_manager ( )
		{
		}
		auto operator ( ) ( )
		{
		}
	} ;
	template < typename ... F >
	constexpr auto output_manager_creator ( F ... f )
	{
		return output_manager < F ... > { f ... } ;
	}
	namespace detail
	{
		template < typename >
		struct pre_format_to_format ;
		template < char ... ch >
		struct pre_format_to_format < sprout::types::basic_string < char , ch ... > >
			: ftmp::list < ftmp::integral < char , ch > ... >
		{
		} ;
		template < typename >
		struct printf ;
		template < typename >
		struct printf_impl_format ;
		template < typename >
		struct printf_impl_normal ;
		template < >
		struct printf < ftmp::list < > >
		{
			constexpr static auto func ( )
			{
				return output_manager_creator ( ) ;
			}
		} ;
		template < char chH , char ... ch >
		struct printf < ftmp::list < ftmp::integral < char , chH > , ftmp::integral < char , ch > ... > >
			: ftmp::if_c
			<
				chH == '%' ,
				printf_impl_format < ftmp::list < ftmp::integral < char , chH > , ftmp::integral < char , ch > ... > > ,
				printf_impl_normal < ftmp::list < ftmp::integral < char , chH > , ftmp::integral < char , ch > ... > >
			>::type
		{
		} ;
		template < char chH , char ... ch >
		struct printf_impl_normal < ftmp::list < ftmp::integral < char , chH > , ftmp::integral < char , ch > ... > >
		{
			template < typename ... T >
			constexpr static auto func ( const T & ... v )
			{
				return output_manager_creator ( lambda < char > ( chH ) , printf < ftmp::list < ftmp::integral < char , ch > ... > >::func ( v ... ) ) ;
			}
		} ;
		template < char ... ch >
		struct printf_impl_format < ftmp::list < ftmp::integral < char , '%' > , ftmp::integral < char , '%' > , ftmp::integral < char , ch > ... > >
		{
			template < typename ... T >
			constexpr static auto func ( const T & ... v )
			{
				return output_manager_creator ( lambda < char > ( '%' ) , printf < ftmp::list < ftmp::integral < char , ch > ... > >::func ( v ... ) ) ;
			}
		} ;
		template < char ... ch >
		struct printf_impl_format < ftmp::list < ftmp::integral < char , '%' > , ftmp::integral < char , 'c' > , ftmp::integral < char , ch > ... > >
		{
			template < typename ... T >
			constexpr static auto func ( char h , const T & ... v )
			{
				return output_manager_creator ( lambda < char > ( h ) , printf < ftmp::list < ftmp::integral < char , ch > ... > >::func ( v ... ) ) ;
			}
		} ;
		template < char ... ch >
		struct printf_impl_format < ftmp::list < ftmp::integral < char , '%' > , ftmp::integral < char , 's' > , ftmp::integral < char , ch > ... > >
		{
			template < std::size_t N , typename ... T >
			constexpr static auto func ( const char ( & h ) [ N ] , const T & ... v )
			{
				return output_manager_creator ( lambda < char [ N ] > ( h ) , printf < ftmp::list < ftmp::integral < char , ch > ... > >::func ( v ... ) ) ;
			}
		} ;
		template < char ... ch >
		struct printf_impl_format < ftmp::list < ftmp::integral < char , '%' > , ftmp::integral < char , 'd' > , ftmp::integral < char , ch > ... > >
		{
			template < typename ... T >
			constexpr static auto func ( int h , const T & ... v )
			{
				return output_manager_creator ( lambda < int > ( h ) , printf < ftmp::list < ftmp::integral < char , ch > ... > >::func ( v ... ) ) ;
			}
		} ;
		template < char ... ch >
		struct printf_impl_format < ftmp::list < ftmp::integral < char , '%' > , ftmp::integral < char , 'f' > , ftmp::integral < char , ch > ... > >
		{
			template < typename ... T >
			constexpr static auto func ( double h , const T & ... v )
			{
				return output_manager_creator ( lambda < double > ( h ) , printf < ftmp::list < ftmp::integral < char , ch > ... > >::func ( v ... ) ) ;
			}
		} ;
	}
	template < typename format_type , typename ... T >
	constexpr auto printf ( const T & ... v )
	{
		return detail::printf < format_type >::func ( v ... ) ;
	}
}
#define TSP_PRINTF(format,...) \
	SPROUT_TYPES_STRING_TYPEDEF ( sprout::to_string( format ) , SPROUT_PP_CAT(pre_format_type , __LINE__) ) ; \
	using SPROUT_PP_CAT ( format_type , __LINE__ ) = typename tsp::detail::pre_format_to_format < SPROUT_PP_CAT ( pre_format_type , __LINE__ )>::type ; \
	tsp::printf < SPROUT_PP_CAT ( format_type , __LINE__ )> ( __VA_ARGS__ ) ( )
#endif