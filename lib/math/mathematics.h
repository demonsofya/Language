/*!
\file
\brief Файл с математическими функциями

Содержит функцию сравнения даблов и константу точности
*/
#ifndef MATHEMATICS_H_INCLUDED
#define MATHEMATICS_H_INCLUDED

/// Точность функции сравнения даблов
const double EPS = 0.001;

/// Функция для сравнения двух даблов
///
/// \return true, если равны, false иначе
bool CompareDoubles(double a, double b);

#endif // MATHEMATICS_H_INCLUDED
