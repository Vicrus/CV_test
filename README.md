## Fill Your Brains!
Напряги мозги, чтобы налить себе стаканчик лимонада ;)

**Как это работает?**

- Вы надеваете нейроинтерфейс

![Вы надеваете нейроинтерфейс](https://github.com/stovv/-Fill-You-Brains/blob/master/imgs/neiro.png?raw=true)
- Видите свое статус того сколько осталось думать чтоб получить лимонад
![enter image description here](https://github.com/stovv/-Fill-You-Brains/blob/master/imgs/face.png?raw=true)
- Думаете о чем-нибудь сложном
- И готово!

![enter image description here](https://github.com/stovv/-Fill-You-Brains/blob/master/imgs/1.png?raw=true)
# Техническая составляющая
В проекте использовались следующие компоненты:
- Электрическая помпа //для подачи воды из 19л бутыли
- Arduino Uno + Реле //для включения помпы
- Адресная светодиодная лента // для наглядного отображения прогресса заполнения 
- Кнопка // для экстренного включения воды
- Нейроинтерфейс NeiroSky с открытым Api 
- Библиотека OpenCV
## Реализация
У нас есть мотор, путем проб и ошибок, мы поняли что выходного тока для нормальной работы мотора мало, а с usb ноутбука вполне, поэтому мы решили использовать реле для включения. Вот так выглядит схема подключения![enter image description here](https://github.com/stovv/-Fill-You-Brains/blob/master/imgs/scheme.png?raw=true)
