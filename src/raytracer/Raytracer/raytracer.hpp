#pragma once
#include<vector>
#include <atomic>

class Raytracer 
{
public:

    //Raytracer() = default;

    void trace(std::vector<unsigned char>& texture);
    int width() const {return m_width;}
    int height() const {return m_height;}
    int samplesPerPixel() const {return m_samples_per_pixel;}

    void setWidth(int width) {m_width = width;}
    void setHeight(int height) {m_height = height;}
    void setSamplesPerPixel(int samples_per_pixel) {m_samples_per_pixel = samples_per_pixel;}
    void stopRendering()
    {
        m_is_rendering = false;
    }

    bool isRendering()
    {
        return m_is_rendering;
    }

private:
    int m_width{1200}, m_height{675}, m_samples_per_pixel{5};
    std::atomic<bool> m_is_rendering{ false };  // Atomic flag to indicate is rendering
};
