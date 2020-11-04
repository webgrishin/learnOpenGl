#include "Camera.h"

namespace RenderEngine {
    // Конструктор, использующий векторы
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Конструктор, использующие скаляры
    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Возвращает матрицу вида, вычисленную с использованием углов Эйлера и LookAt-матрицы 
    glm::mat4 Camera::GetViewMatrix()
    {
        //return myLookAt();
        return glm::lookAt(Position, Position + Front, Up);
    }

    //Обрабатываем входные данные, полученные от любой клавиатуроподобной системы ввода. Принимаем входной параметр в виде определенного камерой перечисления (для абстрагирования его от оконных систем)
    void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        //float velocity = 1;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;

        //Position.y = 0.0f; //Оставться на земле, в плоскости xz
    }

    //Обрабатываем входные данные, полученные от системы ввода с помощью мыши. Ожидаем в качестве параметров значения смещения как в направлении X, так и в направлении Y.
    void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Убеждаемся, что когда тангаж выходит за пределы обзора, экран не переворачивается
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Обновляем значения вектора-прямо, вектора-вправо и вектора-вверх, используя обновленные значения углов Эйлера
        updateCameraVectors();
    }

    // Обрабатывает входные данные, полученные от события колеса прокрутки мыши. Интересуют только входные данные на вертикальную ось колесика 
    void Camera::ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

    // Вычисляет вектор-прямо по (обновленным) углам Эйлера камеры
    void Camera::updateCameraVectors()
    {
        // Вычисляем новый вектор-прямо
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw));// *cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Также пересчитываем вектор-вправо и вектор-вверх
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Нормализуем векторы, потому что их длина становится стремится к 0 тем больше, чем больше вы смотрите вверх или вниз, что приводит к более медленному движению.
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    glm::mat4 Camera::myLookAt()
    {
        glm::vec3 target = Position + Front;
        glm::vec3 front = glm::normalize(Position - target);
        glm::vec3 right = glm::normalize(glm::cross(WorldUp, front));
        glm::vec3 up = glm::cross(front, right);
        //glm::vec3 front = Front * -1.0f;
		//glm::vec3 right = Right * -1.0f;
        //glm::vec3 up = Up * -1.0f;

        glm::mat4 lookAt(1.0f);
        lookAt[0][0] = right.x;
        lookAt[1][0] = right.y;
        lookAt[2][0] = right.z;
        lookAt[0][1] = up.x;
        lookAt[1][1] = up.y;
        lookAt[2][1] = up.z;
        lookAt[0][2] = front.x;
        lookAt[1][2] = front.y;
        lookAt[2][2] = front.z;

        glm::mat4 position(1.0f);
        position[3][0] = -Position.x;
        position[3][1] = -Position.y;
        position[3][2] = -Position.z;
        return lookAt * position;
    }
}