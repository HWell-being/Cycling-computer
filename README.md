# Cycling-computer
A homemade bicycle computer

霍尔传感器               STM32
+------------+                   
|        	   |          +-----------------+
| VCC  --- |------- |    3.3V          |
|GND ----|------- |    GND          |
| OUT  ---|------- | PA0 (EXTI0) |
+------------+         +------------------+



​     蓝牙         		    STM32
+------------+                
|        	   |          +-----------------+
| TXD ----|------- | 	PA3           |
| RXD ----|------- |     PA2           |

+------------+         +------------------+



​    W25Q64                STM32
+------------+           +-----------------+
|   CS	   |------- |    PA4          |
|   CLK     |------- |    PA5          |
|	DO 	|------- |    PA6          |
| 	DI 	 |------- |	PA7 		 |
+------------+         +------------------+



  MPU6050              STM32
+------------+                
|        	   |          +-----------------+
|  SCL      |------- | 	PB0           |
|  SDA     |------- |     PB1           |

+------------+         +------------------+



​     蓝牙         		    STM32
+------------+                
|        	   |          +-----------------+
| TXD ----|------- | 	PB10         |
| RXD ----|------- |     PB11         |

+------------+         +------------------+
