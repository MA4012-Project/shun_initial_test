#pragma config(Sensor, dgtl1, limitSwitch, sensorTouch)
#pragma config(Motor, port2, testMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor, port3, releaseServo, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor, port1, leftWheel, tmotorVex393_HBridge, openLoop)
#pragma config(Motor, port10, rightWheel, tmotorVex393_HBridge, openLoop)

const int MOTOR_POWER = 100;

// Independent wheel powers; preserves your previous directions.
const int LEFT_WHEEL_POWER = -90;
const int RIGHT_WHEEL_POWER = 90;

const int SERVO_OUT_POWER = -90;
const int OUT_TIME_MS = 500;

task main()
{
    bool triggered = false;

    motor[testMotor] = 0;
    motor[releaseServo] = 0;
    motor[leftWheel] = 0;
    motor[rightWheel] = 0;

    while (true)
    {
        // Main motor keeps running.
        motor[testMotor] = MOTOR_POWER;

        // Latch the first limit-switch detection.
        if (!triggered && SensorValue[limitSwitch] == 1)
        {
            triggered = true;
            clearTimer(T2);
        }

        // Drive until detection, then remain stopped.
        if (!triggered)
        {
            motor[leftWheel] = LEFT_WHEEL_POWER;
            motor[rightWheel] = RIGHT_WHEEL_POWER;
        }
        else
        {
            motor[leftWheel] = 0;
            motor[rightWheel] = 0;
        }

        // Servo sequence runs once after detection.
        if (triggered)
        {
            if (time1[T2] < 2000)
            {
                motor[releaseServo] = 0;
            }
            else if (time1[T2] < 2000 + OUT_TIME_MS)
            {
                motor[releaseServo] = SERVO_OUT_POWER;
            }
            else
            {
                motor[releaseServo] = 0;
            }
        }

        wait1Msec(20);
    }
}
