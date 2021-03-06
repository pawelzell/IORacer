#pragma once


#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>
#include <CheckPointObserver.h>
#include <TimeManager.h>
#include <ObjectTypes.h>

/**
 * Store list of chekpoints. Track and update active checkpoint.
 */
class CheckPointManager : public CheckPointObserver {
public:
    CheckPointManager(VehiclePtr player, std::vector<CheckPointPtr> checkPoints, int totalLaps);

    /**
     * Reset active checkpoint to first on the list.
     */
    void Reset();

    /**
     * When function is called move active checkpoint index to next checkpoint
     * if current active checkpoint was already set to disabled.
     * Call SetEnable(true) on new active checkpoint.
     */
    void NotifyCheckPointReached();

    void Update(float dt);

    sf::Vector2f GetNextCheckPointPosition() const;

    TimeManagerPtr GetTimeManager() const;

    bool NewSectorBeginNotify() const;

    bool NewLapBeginNotify() const;

    int GetTotalNumberOfLaps() const;

    int GetTotalNumberOfSectors() const;

    int GetCurrentLapNumber() const;

    int GetCurrentSectorNumber() const;

private:
    CheckPointPtr GetCurrentCheckPoint() const;

    std::vector<CheckPointPtr> checkPoints_;

    TimeManagerPtr timeManager_;
    /// Current sector number
    int currentSector_;
    /// Total number of track's sectors
    int totalSectors_;
    /// Current lap number
    int currentLap_;
    /// Total number of laps in race
    int totalLaps_;

    VehiclePtr player_;
};
