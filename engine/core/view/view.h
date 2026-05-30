#pragma once
#include "core/math/mat4.h"
namespace Boo
{

    enum class UIDesignFitMode;
    class View
    {
    private:
        /**
         * @brief 设计宽度
         */
        int _designWidth;
        /**
         * @brief 设计高度
         */
        int _designHeight;
        /**
         * @brief 设计适配模式
         */

        UIDesignFitMode _designFitMode;
        /**
         * @brief 适配矩阵
         */
        Mat4 _fitMat;

        /**
         * @brief 视图实际宽度
         */
        int _width;
        /**
         * @brief 视图实际高度
         */
        int _height;

    private:
        void _updateFitMat();

    public:
        View();
        View(int width, int height);
        /**
         * @brief 重新设置视图大小
         * @param width 视图宽度
         * @param height 视图高度
         */
        void resize(int width, int height);
        int getWidth() const;
        int getHeight() const;
        void setDesignFitMode(UIDesignFitMode fitMode);
        void setDesignSize(int width, int height);
        int getDesignWidth() const;
        int getDesignHeight() const;
        UIDesignFitMode getDesignFitMode() const;
        /**
         * @brief 获取适配矩阵
         * @return Mat4 适配矩阵
         */
        const Mat4 &getFitMatrix() const;
        ~View();
    };

} // namespace Boo