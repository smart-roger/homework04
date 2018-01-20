#include <iostream>

#include <array>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <type_traits>

/*!
	Шаблонная функция для печати айпишника из контейнера
*/

/*! Реализация для целых чисел (char, int, long)
	контейнеры, double и строки не рассматриваются
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

/*!	Реализация функции для строк. Выводится "как есть"
*/
template <class T>
typename std::enable_if< std::is_same<T, std::string>::value, void>::type
printIP (const T& val, std::ostream& stream){
	stream << val << std::endl;
}

/*!	Реализация вывода на печать адреса, когда аргумент - контейнер
	Для элементов контейнера вызывается метод std::to_string. 
	Необходимо для корректного вывода значений типов char (иначе выводятся не числа, а символы).
*/
template<class T>
typename std::enable_if<!std::is_integral <T>::value &&
						!std::is_same<T, std::string>::value
					,void>::type 
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

/*!
	Запуск основной программы - только примеры
*/
int main(int argc, char const *argv[])
{
    try
    {
		//	Тестирование вывода целочисленных параметров		
		printIP(char(-1), std::cout);
		printIP(short(0), std::cout);
		printIP(int(2130706433), std::cout);
		printIP((long long)(8875824491850138409), std::cout);
		//	Тестирование вывода строки
		printIP(std::string("1.2.3.4.5.6.7.8.99"), std::cout);
				
		//	Тестирование вывода контейнеров с числами
		printIP(std::vector<int>{0x012345,0x987654,0x112233}, std::cout);
		printIP(std::vector<unsigned char>{0x11,0x22,0x33,0x44}, std::cout);
		printIP(std::list<int>{0x012345,0x987654,0x112233}, std::cout);
		printIP(std::list<long long>{75824491850138409,75824491850138408,75824491850138410,75824491850138407}, std::cout);
				
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
