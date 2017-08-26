#include "World.h"

#include <iostream>

#include "Random.h"
#include "Common.h"

World::World()
:   m_people(WIDTH * HEIGHT)
{
    m_worldTexture.loadFromFile("res/world_map.png");
    m_world.setTexture  (&m_worldTexture);
    m_world.setSize     ({1280, 720});

    m_worldImage.loadFromFile("res/world_map.png");
    m_worldTexture.loadFromImage(m_worldImage);

    //Fill the map with dead people (no colour)
    createColonies();
}

void World::update()
{
    std::vector<Person> newPeople(WIDTH * HEIGHT);

    for (unsigned y = 0; y < HEIGHT; y++)
    for (unsigned x = 0; x < WIDTH; x++)
    {
        auto& person = m_people[getIndex(x, y)];

        if (!person.getData().isAlive)
            continue;


        int xMoveTo = x + Random::get().intInRange(-1, 1);
        int yMoveTo = y + Random::get().intInRange(-1, 1);

        if (xMoveTo < 0 || xMoveTo >= (int)WIDTH) continue;
        if (yMoveTo < 0 || yMoveTo >= (int)HEIGHT) continue;



    }
}

const sf::Color& World::getColorAt(unsigned x, unsigned y)
{
    return m_colonies[m_people[getIndex(x, y)].getData().colony].colour;
}


void World::draw(sf::RenderWindow& window)
{
    window.draw(m_world);
}

void World::createColonies()
{
    //First colony type is just nothingness
    int id = 0;
    m_colonies[0].colour = {0, 0, 0, 0};
    m_colonies[0].id = id++;

    //Set up the colony data and choose locations for the colonies
    std::array<sf::Vector2i, 10> colonyLocations;
    for (unsigned i = 1; i < m_colonies.size(); i++)
    {
        auto& colony = m_colonies[i];
        colony.id = id++;

        //Get colony colour, randomly decide the "colour type" of it
        auto colour = (uint8_t)Random::get().intInRange(50, 200);
        switch (Random::get().intInRange(0, 2))
        {
            case 0:
                colony.colour = {colour / 2, colour, colour};
                break;

            case 1:
                colony.colour = {colour, colour / 2, colour};
                break;

            case 2:
                colony.colour = {colour, colour, colour / 2};
                break;

        }


        //Find a on-land location for the colony to originate from
        int x, y;
        bool locationFound = false;
        while (!locationFound)
        {
            x = Random::get().intInRange(0, WIDTH);
            y = Random::get().intInRange(0, HEIGHT);

            auto pixel = m_worldImage.getPixel(x, y);
            if (pixel.g >= 250)
            {
                locationFound = true;
                colonyLocations[i] = {x, y};
            }
        }
    }

    //Place colonies at the locations
    for (unsigned i = 1; i < m_colonies.size(); i++)
    {
        auto& location = colonyLocations[i];
        //place up to 50 people at the location
        for (int j = 0; j < 50; j++)
        {
            int xOffset = Random::get().intInRange(-4, 4);
            int yOffset = Random::get().intInRange(-4, 4);

            int newLocationX = xOffset + location.x;
            int newLocationY = yOffset + location.y;

            if (newLocationX < 0 || newLocationX >= (int)WIDTH) continue;
            if (newLocationY < 0 || newLocationY >= (int)HEIGHT) continue;
            if (m_worldImage.getPixel(newLocationX, newLocationY).g < 250) continue;

            PersonData data;
            data.age        = 0;
            data.strength   = Random::get().intInRange(400, 500);
            data.isAlive    = true;
            data.colony     = i;

            m_people[getIndex(newLocationX, newLocationY)].init(data);

        }
    }
}
























