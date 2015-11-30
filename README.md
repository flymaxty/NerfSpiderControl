# NerfSpiderControl
NerfSpiderControl用于控制基于Intel Edison控制板改造的Nerf遥控蜘蛛机器人。

##License
NerfSpiderControl can be used under the Apache License v2.0.

##Basic Function
1. 机器人的行进。
2. 机器人炮台的旋转和俯仰。
3. 炮弹的发射。
4. 激光器的开关。

##Dependency
1. mraa 0.8.0
2. OpenCV 3.0

##Compile
该程序需要在Intel Edison 下面完成编译，编译使用CMake完成。

```bash
cd NerfSpiderControl
mkdir build
cd build
cmake ..
make
```

##Sample
```bash
cd NerfSpiderControl
cd bin

# 炮台旋转控制
# 参数：（速度：±（0-1），正数为逆时针转，负数为顺时针转）（步数：1-∞的整数）
./SampleBarbette 1 20

# 激光器开关控制
# 参数：（开关：0或1）
./SampleLserControl 1

# 炮弹发射控制
# 参数（射击时间：1-∞的整数）
./SampleShootControl 5

# 炮台的俯仰控制，因为炮台电机是单向运动的，所以只能控制运动时间。
# 参数：（俯仰电机运动时间：1-∞的整数）
./SampleShooterControl 4

#机器人行进控制，机器人只能朝炮台正方向前进。
#参数：（速度：0-1，整数）（运动时间：1-∞，整数）
./SampleWalk 1 5

##人脸识别，机器人看见人脸后，会朝人脸方向前进。
##算法不是很稳定，待优化。
./SampleFaceTrack

##几何图形识别，机器人看见黑色圆形后，会朝其方向行走。
##相比人脸识别，圆形的识别速度和稳定性都比较好（仍需
##要改进），相关图片在resource目录里。
./SampleGeometryDetect
```
