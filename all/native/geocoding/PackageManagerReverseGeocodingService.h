/*
 * Copyright (c) 2016 CartoDB. All rights reserved.
 * Copying and using this code is allowed only according
 * to license terms, as given in https://cartodb.com/terms/
 */

#ifndef _CARTO_PACKAGEMANAGERREVERSEGEOCODINGSERVICE_H_
#define _CARTO_PACKAGEMANAGERREVERSEGEOCODINGSERVICE_H_

#if defined(_CARTO_GEOCODING_SUPPORT) && defined(_CARTO_PACKAGEMANAGER_SUPPORT)

#include "geocoding/ReverseGeocodingService.h"
#include "packagemanager/PackageManager.h"

#include <memory>

namespace sqlite3pp {
    class database;
}

namespace carto {
    namespace geocoding {
        class RevGeocoder;
    }

    /**
     * A reverse geocoding service that uses geocoding packages from package manager.
     */
    class PackageManagerReverseGeocodingService : public ReverseGeocodingService {
    public:
        /**
         * Constructs a new instance of the PackageManagerReverseGeocodingService given package manager instance.
         * @param packageManager The package manager instance to use.
         */
        explicit PackageManagerReverseGeocodingService(const std::shared_ptr<PackageManager>& packageManager);
        virtual ~PackageManagerReverseGeocodingService();

        /**
         * Returns the search radius (in meters).
         * @return The search radius in meters.
         */
        float getSearchRadius() const;
        /**
         * Sets the search radius (in meters).
         * The default search radius is 100 meters.
         * @param radius The new search radius in meters.
         */
        void setSearchRadius(float radius);

        virtual std::vector<std::shared_ptr<GeocodingResult> > calculateAddresses(const std::shared_ptr<ReverseGeocodingRequest>& request) const;

    protected:
        class PackageManagerListener : public PackageManager::OnChangeListener {
        public:
            PackageManagerListener(PackageManagerReverseGeocodingService& service);
        		
            virtual void onPackagesChanged();
        		
        private:
            PackageManagerReverseGeocodingService& _service;
        };

        const std::shared_ptr<PackageManager> _packageManager;
        float _searchRadius;

        mutable std::map<std::shared_ptr<PackageInfo>, std::shared_ptr<sqlite3pp::database> > _cachedPackageDatabaseMap;
        mutable std::shared_ptr<geocoding::RevGeocoder> _cachedRevGeocoder;

        mutable std::mutex _mutex;

    private:
        std::shared_ptr<PackageManagerListener> _packageManagerListener;

        static const float DEFAULT_SEARCH_RADIUS;
    };
    
}

#endif

#endif
