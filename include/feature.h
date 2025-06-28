//
// Created by backo on 27/05/25.
//

#ifndef DPP_STRUCTURES_FEATURE_H
#define DPP_STRUCTURES_FEATURE_H

#include <dpp/cluster.h>
#include <vector>

namespace dpp_structures {

    /**
     * Abstract class for creating classes with a method that is run when the bot starts.
     *
     * You can leave the setup method unimplemented. This way, this class can be used only for semantic purposes.
     */
    class feature {
    private:
        static std::vector<feature*> features;

        void attach(dpp::cluster* _bot);

    protected:
        dpp::cluster* bot = nullptr;

        /**
         * It is guaranteed that when this method is executed, this->bot != nullptr.
         */
        virtual void setup();

    public:

        // Polymorphism, let's fucking go
        static void setup_all(dpp::cluster& bot);

        feature();
    };

}// namespace features

#endif//DPP_STRUCTURES_FEATURE_H
