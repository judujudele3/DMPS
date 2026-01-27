#ifndef RESULTDISPLAYMANAGER_HPP
#define RESULTDISPLAYMANAGER_HPP

#include "IResultValueRenderer.hpp"
#include "BasicResultRenderers.hpp"
#include <vector>
#include <memory>
#include <algorithm>

/**
 * @brief Manages rendering of result values
 *
 * This class:
 * - Maintains a registry of renderers
 * - Selects the appropriate renderer for each value type
 * - Provides a unified interface for rendering
 *
 * Extensibility: New renderers can be registered at any time.
 */
class ResultDisplayManager {
public:
    ResultDisplayManager() {
        // Register default renderers
        registerDefaultRenderers();
    }

    /**
     * @brief Register a custom renderer
     * @param renderer Shared pointer to the renderer
     */
    void registerRenderer(std::shared_ptr<IResultValueRenderer> renderer) {
        renderers_.push_back(renderer);

        // Sort by priority (higher first)
        std::sort(renderers_.begin(), renderers_.end(),
                  [](const auto& a, const auto& b) {
                      return a->getPriority() > b->getPriority();
                  });
    }

    /**
     * @brief Render a result value as a string
     * @param key The key/name of the result
     * @param value The result value
     * @return Rendered string
     */
    QString render(const std::string& key, const ResultValue& value) const {
        for (const auto& renderer : renderers_) {
            if (renderer->canRender(value)) {
                return renderer->render(key, value);
            }
        }

        // Should never happen due to FallbackRenderer
        return QString::fromStdString(key + ": <error>");
    }

    /**
     * @brief Create a detail widget for a result value (if available)
     * @param key The key/name of the result
     * @param value The result value
     * @return Widget or nullptr if no detail widget available
     */
    QWidget* createDetailWidget(const std::string& key, const ResultValue& value) const {
        for (const auto& renderer : renderers_) {
            if (renderer->canRender(value)) {
                return renderer->createDetailWidget(key, value);
            }
        }
        return nullptr;
    }

private:
    std::vector<std::shared_ptr<IResultValueRenderer>> renderers_;

    void registerDefaultRenderers() {
        // Register basic renderers (order matters due to priority)
        registerRenderer(std::make_shared<IntRenderer>());
        registerRenderer(std::make_shared<SizeTRenderer>());
        registerRenderer(std::make_shared<DoubleRenderer>());
        registerRenderer(std::make_shared<StringRenderer>());
        registerRenderer(std::make_shared<BoolRenderer>());
        registerRenderer(std::make_shared<VectorDoubleRenderer>());
        registerRenderer(std::make_shared<VectorStringRenderer>());

        // Fallback renderer (lowest priority)
        registerRenderer(std::make_shared<FallbackRenderer>());
    }
};

#endif // RESULTDISPLAYMANAGER_HPP
