#include "view.h"
#include "boo.h"
#include "core/game.h"
namespace Boo
{

    View::View() : _width(0), _height(0) {}

    View::View(int width, int height) : _width(width),
                                        _height(height),
                                        _designWidth(width),
                                        _designHeight(height),
                                        _designFitMode(UIDesignFitMode::None),
                                        _fitMat(Mat4::identity())
    {
        // 根据设计分辨率，适配模式，视图分辨率，计算适配矩阵
        this->_updateFitMat();
    }
    void View::setDesignFitMode(UIDesignFitMode fitMode)
    {
        this->_designFitMode = fitMode;
        this->_updateFitMat();
    }
    void View::setDesignSize(int width, int height)
    {
        this->_designWidth = width;
        this->_designHeight = height;
        this->_updateFitMat();
    }
    void View::resize(int width, int height)
    {
        this->_width = width;
        this->_height = height;
        this->_updateFitMat();
    }
    void View::_updateFitMat()
    {
        // 根据设计分辨率，适配模式，视图分辨率，计算适配矩阵
        float scaleX = (float)_width / (float)_designWidth;
        float scaleY = (float)_height / (float)_designHeight;
        switch (_designFitMode)
        {
        case UIDesignFitMode::None:
            scaleX = 1.0f;
            scaleY = 1.0f;
            this->_designWidth = this->_width;
            this->_designHeight = this->_height;
            break;
        case UIDesignFitMode::Width:
            scaleY = scaleX;
            break;
        case UIDesignFitMode::Height:
            scaleX = scaleY;
            break;
        default:
            break;
        }
        // LOGI("scaleX: %f", scaleX);
        // LOGI("scaleY: %f", scaleY);
        this->_fitMat.setM0(scaleX);
        this->_fitMat.setM5(scaleY);
        this->_fitMat.setM11(1.0f);
    }
    int View::getDesignWidth() const
    {
        return _designWidth;
    }
    int View::getDesignHeight() const
    {
        return _designHeight;
    }

    UIDesignFitMode View::getDesignFitMode() const
    {
        return _designFitMode;
    }

    int View::getWidth() const
    {
        return _width;
    }

    int View::getHeight() const
    {
        return _height;
    }
    const Mat4 &View::getFitMatrix() const
    {
        return _fitMat;
    }

    View::~View() {}

} // namespace Boo