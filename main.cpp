#include <iostream>
#include <array>
#include <vector>
#include <type_traits>
#include <limits>
#include <cstring>

/*!
	Шаблонная функция для печати айпишника из контейнера
*/

/*! Реализация для целых чисел (char, int, long)
	double и строки не рассматриваются
*/


template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
printIP(const T& val, std::ostream& stream){
	//	Определим сколько байт в переданном типе
	constexpr size_t len(sizeof(T));
	static_assert(len>0," sizeof returned negative value?! ");
	
	//	Массив байт для представления адреса
	std::array<unsigned char, len> bytes;
	//	Грязно, но удобно заполнить массив и работать с памятью напрмяую
	std::memcpy(bytes.data(), &val, len);
	
	for(auto i=len-1;i>0;--i)
		stream << std::to_string(bytes.at(i)) << ".";
	stream << std::to_string(bytes.at(0));
	stream << std::endl;
}


/*!	Реализация вывода на печать адреса, когда аргумент - контейнер с целыми числами
	Для элементов контейнера вызывается метод std::to_string. Необходимо для корректного вывода значений типов char.
*/
template<typename T>
typename std::enable_if<!std::is_integral <T>::value&&
						std::is_integral <typename T::value_type>::value, 
void>::type 
printIP(const T& container, std::ostream& stream){
	//	Итераторы для работы с контейнерами
	auto first = container.begin();
	auto last = container.end();
	
	//	Пропускаем ситуацию с пустым контейнером?
	if(first!=last){
		//	Перебираем все элементы контейнера кроме последнего
		for(--last; first!=last; ++first)
			stream << std::to_string(*first) <<".";
		//	Выводим последний элемент контейнера
		stream << std::to_string(*first) <<std::endl;
	}
}

/*!	Реализация вывода на печать адреса, когда аргумент - контейнер НЕ с целыми числами (например, строками)
	Элементы контейнера выводятся непосредственно в переданный поток.
*/
template<typename T>
typename std::enable_if<!std::is_integral <T>::value &&
						!std::is_integral <typename T::value_type>::value, 
void>::type 
printIP(const T& container, std::ostream& stream){
	auto first = container.begin();
	auto last = container.end();
	
	if(first!=last){
		
		for(--last; first!=last; ++first)
			stream << *first <<".";
		stream << *first <<std::endl;
	}
}

/*!
	Запуск основной программы - только примеры
*/
int main(int argc, char const *argv[])
{
    try
    {
		//	Тестирование вывода целочисленных параметров
		char test_ch (0x01);
		printIP(test_ch, std::cout);
		
		short test_short(0x0123);
		printIP(test_short, std::cout);
		
		int test_int(0x012345);
		printIP(test_int, std::cout);
		
		long long test_ll(0x0123456789ABCDEF);
		printIP(test_ll, std::cout);
				
		//	Тестирование вывода контейнеров с числами
		printIP(std::vector<int>{0x012345,0x987654,0x112233}, std::cout);
		printIP(std::vector<unsigned char>{0x11,0x22,0x33,0x44}, std::cout);
		printIP(std::vector<int>{0x012345,0x987654,0x112233}, std::cout);
		printIP(std::vector<long>{1,2,3,4}, std::cout);
		
		//	Тестирование вывода контейнеров со строками
		printIP(std::vector<std::string>{"91","82","73","214"}, std::cout);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
