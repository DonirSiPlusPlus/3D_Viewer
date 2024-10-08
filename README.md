# 3DViewer v2.0

Программа для визуализации каркасной модели в трехмерном пространстве на языке С++:

 - Релизовано перемещение модели на заданное расстояние относительно осей X, Y, Z.
 - Релизовано вращение модели на заданный угол относительно своих осей X, Y, Z
 - Релизовано масштабирование модели на заданное значение.
 - Программа позволяет настраивать тип проекции (параллельная и центральная)
 - Программа позволяет настраивать тип (сплошная, пунктирная), цвет и толщину ребер, способ отображения (отсутствует, круг, квадрат), цвет и размер вершин
 - Программа позволяет выбирать цвет фона
 - Настройки сохраняются между перезапусками программы

Завершено 14.03.2024

## Information

Использован архитектурный паттерн MVC, а также хранитель, фасад, стратегия. Сами модели необходимо загружать из файлов формата .obj. 

### Формат представления описаний трехмерных объектов .obj

Файлы .obj - это формат файла описания геометрии, впервые разработанный компанией Wavefront Technologies. Формат файла открыт и принят многими поставщиками приложений для 3D-графики.

Формат файла .obj - это простой формат данных, который представляет только трехмерную геометрию, а именно положение каждой вершины, положение UV координат текстуры каждой вершины, нормали вершин и грани, которые определяют каждый многоугольник как список вершин и вершин текстуры. Координаты obj не имеют единиц измерения, но файлы obj могут содержать информацию о масштабе в удобочитаемой строке комментариев.

Пример файла формата .obj:
```
  # Список вершин, с координатами (x, y, z[, w]), w является не обязательным и по умолчанию равен 1.0
  v 0.123 0.234 0.345 1.0
  v ...
  ...
  # Определения поверхности (сторон)
  f v1 v2 v3
  f ...
  ...
```
