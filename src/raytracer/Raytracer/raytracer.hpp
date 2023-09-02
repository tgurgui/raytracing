#pragma once
#include<vector>

class Raytracer 
{
public:

    Raytracer() = default;

    void trace(std::vector<unsigned char>& texture);
    int width() const {return m_width;}
    int height() const {return m_height;}
    int samplesPerPixel() const {return m_samples_per_pixel;}

    void setWidth(int width) {m_width = width;}
    void setHeight(int height) {m_height = height;}
    void setSamplesPerPixel(int samples_per_pixel) {m_samples_per_pixel = samples_per_pixel;}
private:
    int m_width{1200}, m_height{675}, m_samples_per_pixel{5};
};
