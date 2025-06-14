// Written by malekpour-dev.ir
// SpinnerTemplates is a class responsible for storing and managing spinner templates.

#pragma once

#include "CanvasWidget.h"
#include <QString>
#include <vector>

struct SpinnerTemplateItem {
    SpinnerType type;
    SpinnerAnimation anim;
    QPointF position;
    int size;
    QString color;
    float speed;
    float duration;
    float preDelay;
    float postDelay;
};

struct SpinnerTemplate {
    QString name;
    QString description;
    std::vector<SpinnerTemplateItem> items;
};

class SpinnerTemplates {
public:
    static const std::vector<SpinnerTemplate>& getTemplates() {
        static const std::vector<SpinnerTemplate> templates = {
           
            {
                "Loading Bar",
                "Modern segmented loading bar with smooth animation",
                {
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(20, 50), 8, "#2196F3", 45.0f, 1.2f, 0.0f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(35, 50), 8, "#2196F3", 45.0f, 1.2f, 0.2f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(50, 50), 8, "#2196F3", 45.0f, 1.2f, 0.4f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(65, 50), 8, "#2196F3", 45.0f, 1.2f, 0.6f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(80, 50), 8, "#2196F3", 45.0f, 1.2f, 0.8f, 0.3f}
                }
            },
            {
                "Spinning Squares",
                "Elegant rotating squares pattern",
                {
                    {SpinnerType::Square, SpinnerAnimation::Rotate, QPointF(50, 40), 6, "#2196F3", 60.0f, 1.0f, 0.0f, 0.2f},
                    {SpinnerType::Square, SpinnerAnimation::Rotate, QPointF(60, 50), 6, "#2196F3", 60.0f, 1.0f, 0.2f, 0.2f},
                    {SpinnerType::Square, SpinnerAnimation::Rotate, QPointF(50, 60), 6, "#2196F3", 60.0f, 1.0f, 0.4f, 0.2f},
                    {SpinnerType::Square, SpinnerAnimation::Rotate, QPointF(40, 50), 6, "#2196F3", 60.0f, 1.0f, 0.6f, 0.2f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(50, 50), 6, "#2196F3", 60.0f, 1.0f, 0.8f, 0.2f}
                }
            },
            {
                "Progress Ring",
                "Modern circular progress indicator",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 40, "#2196F3", 45.0f, 2.0f, 0.0f, 0.5f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 35, "#E3F2FD", 45.0f, 2.0f, 0.5f, 0.5f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 30, "#2196F3", 45.0f, 2.0f, 1.0f, 0.5f},
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(50, 50), 25, "#E3F2FD", 45.0f, 2.0f, 1.5f, 0.5f}
                }
            },
            {
                "Pulse Grid",
                "Subtle pulsing grid pattern",
                {
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(30, 30), 12, "#2196F3", 35.0f, 1.5f, 0.0f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(50, 30), 12, "#2196F3", 35.0f, 1.5f, 0.2f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(70, 30), 12, "#2196F3", 35.0f, 1.5f, 0.4f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(30, 50), 12, "#2196F3", 35.0f, 1.5f, 0.6f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(50, 50), 12, "#2196F3", 35.0f, 1.5f, 0.8f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(70, 50), 12, "#2196F3", 35.0f, 1.5f, 1.0f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(30, 70), 12, "#2196F3", 35.0f, 1.5f, 1.2f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(50, 70), 12, "#2196F3", 35.0f, 1.5f, 1.4f, 0.3f},
                    {SpinnerType::Square, SpinnerAnimation::Scale, QPointF(70, 70), 12, "#2196F3", 35.0f, 1.5f, 1.6f, 0.3f}
                }
            },
            {
                "Metro Spinner",
                "Minimal metro-style spinner with sliding bars",
                {
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(40, 50), 10, "#607D8B", 50.0f, 1.0f, 0.0f, 0.2f},
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(50, 50), 10, "#607D8B", 50.0f, 1.0f, 0.2f, 0.2f},
                    {SpinnerType::Square, SpinnerAnimation::Slide, QPointF(60, 50), 10, "#607D8B", 50.0f, 1.0f, 0.4f, 0.2f}
                }
            },
            {
                "Professional Dots",
                "Three dots fading in and out in sequence",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(40, 50), 8, "#455A64", 40.0f, 1.0f, 0.0f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(50, 50), 8, "#455A64", 40.0f, 1.0f, 0.2f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(60, 50), 8, "#455A64", 40.0f, 1.0f, 0.4f, 0.2f}
                }
            },
            {
                "Material Progress",
                "Google Material Design style progress indicator",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(50, 50), 40, "#2196F3", 60.0f, 2.0f, 0.0f, 0.0f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 35, "#E3F2FD", 60.0f, 2.0f, 0.0f, 0.0f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 25, "#BBDEFB", 60.0f, 2.0f, 0.0f, 0.0f}
                }
            },
            {
                "Bouncing Dots",
                "Modern bouncing dots loader",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Bounce, QPointF(35, 50), 8, "#FF4081", 50.0f, 0.8f, 0.0f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Bounce, QPointF(50, 50), 8, "#F50057", 50.0f, 0.8f, 0.2f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Bounce, QPointF(65, 50), 8, "#C51162", 50.0f, 0.8f, 0.4f, 0.2f}
                }
            },
            {
                "Pulse Ring",
                "Elegant pulsing ring animation",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 40, "#00BCD4", 30.0f, 2.0f, 0.0f, 0.0f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 35, "#26C6DA", 30.0f, 2.0f, 0.5f, 0.0f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 30, "#4DD0E1", 30.0f, 2.0f, 1.0f, 0.0f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 25, "#80DEEA", 30.0f, 2.0f, 1.5f, 0.0f}
                }
            },
            {
                "Wave Dots",
                "Smooth wave-like dot animation",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Slide, QPointF(20, 50), 8, "#4CAF50", 40.0f, 1.0f, 0.0f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Slide, QPointF(35, 50), 8, "#66BB6A", 40.0f, 1.0f, 0.2f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Slide, QPointF(50, 50), 8, "#81C784", 40.0f, 1.0f, 0.4f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Slide, QPointF(65, 50), 8, "#A5D6A7", 40.0f, 1.0f, 0.6f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Slide, QPointF(80, 50), 8, "#C8E6C9", 40.0f, 1.0f, 0.8f, 0.2f}
                }
            },
            {
                "Pulse Dots",
                "Smooth pulsing dots animation",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(35, 50), 8, "#3F51B5", 40.0f, 1.0f, 0.0f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 8, "#5C6BC0", 40.0f, 1.0f, 0.2f, 0.2f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(65, 50), 8, "#7986CB", 40.0f, 1.0f, 0.4f, 0.2f}
                }
            },
            {
                "Bouncing Grid",
                "A grid of bouncing squares",
                {
                    {SpinnerType::Square, SpinnerAnimation::Bounce, QPointF(50, 40), 10, "#FF6B6B", 70.0f, 1.0f, 0.0f, 0.5f},
                    {SpinnerType::Square, SpinnerAnimation::Fade, QPointF(50, 50), 10, "#4ECDC4", 70.0f, 1.0f, 0.2f, 0.5f},
                }
            },
             {
                "Pulsing Wave",
                "Concentric circles with scale animation",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 50, "#FF6B6B", 40.0f, 1.5f, 0.0f, 0.5f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 35, "#4ECDC4", 40.0f, 1.5f, 0.2f, 0.5f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 20, "#FFE66D", 40.0f, 1.5f, 0.4f, 0.5f},
                    {SpinnerType::Circle, SpinnerAnimation::Scale, QPointF(50, 50), 5, "#1A535C", 40.0f, 1.5f, 0.6f, 0.5f}
                }
            },
            {
                "Dual Ring",
                "Two counter-rotating rings for a professional look",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Rotate, QPointF(50, 50), 40, "#1976D2", 60.0f, 1.5f, 0.0f, 0.0f},
                    {SpinnerType::Circle, SpinnerAnimation::Rotate, QPointF(50, 50), 30, "#90CAF9", -60.0f, 1.5f, 0.0f, 0.0f}
                }
            },
            {
                "Segmented Circle",
                "Segmented circular spinner with fading segments",
                {
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(50, 20), 8, "#1976D2", 60.0f, 1.2f, 0.0f, 0.1f},
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(80, 50), 8, "#1976D2", 60.0f, 1.2f, 0.2f, 0.1f},
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(50, 80), 8, "#1976D2", 60.0f, 1.2f, 0.4f, 0.1f},
                    {SpinnerType::Circle, SpinnerAnimation::Fade, QPointF(20, 50), 8, "#1976D2", 60.0f, 1.2f, 0.6f, 0.1f}
                }
            }
        };
        return templates;
    }
}; 