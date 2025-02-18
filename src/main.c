#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum
{
    FOOD,
    ELSE,
};
enum
{
    WHEAT_FARM,
};
enum
{
    LOWER_STRATA,
    MIDDLE_STRATA,
    UPPER_STRATA,
};
const float wheat_farm_scale = 1;
const float wheet_farm_base_production = 5000;
const float wheet_farm_base_pop_usage = 800;

typedef struct
{
    int id;
    char *name;
    float price;
    float demand;
    float production;
    float base_price;
    int type;
} market_good;

typedef struct
{
    market_good *goods;
    int goods_count;
} local_market;

typedef struct
{
    market_good *output_goods;
    market_good *input_goods;
    int pop_usage;
    int type;
    int level;
} building_t;

typedef struct
{
    int owner_id;
    int id;
    char *name;
    float population;
    float population_growth;
    local_market market;
    building_t *buildings;
} province_t;

typedef struct
{
    province_t *provinces;
    market_good *national_goods;
    int goods_count;
} market;

typedef struct ID_Handler
{
    int curr_id;
    int off;
} ID_Handler;

struct ID_Handler handler = (struct ID_Handler){0, 1};

int next_id()
{
    int id = handler.curr_id + handler.off;
    handler.curr_id += handler.off;
    return id;
}

market_good create_good(char *name, float base_price, int type)
{
    market_good good;
    good.id = next_id();
    good.name = name;
    good.base_price = base_price;
    good.demand = 0;
    good.production = 0;
    good.price = base_price;
    good.type = type;
    return good;
}

char *good_to_string(market_good good)
{
    char buffer[10000] = {0};
    sprintf(buffer, "___________________________\n\nMarket Good: %s\nPrice: %f\nBase Price: %f\nProduction: %f\nDemand: %f\nType: %d\n___________________________", good.name, good.price, good.base_price, good.production, good.demand, good.type);
    char *s = malloc(strlen(buffer) + 1);
    s[strlen(buffer)] = 0;
    memcpy(s, buffer, strlen(buffer));
    return s;
}

const float per_pop_food_usage = 1;
const float food_price_plasticity = 0.5;

void process_local_market(province_t *province)
{
    int i = 0;

    for (; i < province->market.goods_count; ++i)
    {
        if (province->market.goods[i].type == FOOD)
        {
            province->market.goods[i].production = 5000;
            province->market.goods[i].demand = per_pop_food_usage * province->population;
            province->market.goods[i].price = province->market.goods[i].base_price * pow((province->market.goods[i].demand / province->market.goods[i].production), food_price_plasticity);
        }
    }
}

local_market create_local_market()
{
    local_market market;
    market.goods = malloc(sizeof(market_good));
    market.goods_count = 0;
    return market;
}

province_t create_province(char *name)
{
    province_t province;
    province.id = next_id();
    province.market = create_local_market();
    province.population = 1000000;
    province.population_growth = 105;
    province.name = name;
    province.owner_id = 0;
    province.buildings = malloc(sizeof(building_t));
    return province;
}
// time step is from 0 to 1, 1 being a full year 0 being no time.
void handle_population(province_t *province, float time_step)
{
    province->population += ((province->population_growth / 100) * province->population) * time_step;
}

void add_good(province_t *province, market_good good)
{
    for (int i = 0; i < province->market.goods_count; ++i)
    {
        if (province->market.goods[i].id == good.id)
        {
            return;
        }
    }
    province->market.goods = realloc(province->market.goods, sizeof(market_good) * (province->market.goods_count + 1));
    province->market.goods[province->market.goods_count] = good;
    ++province->market.goods_count;
}
void print_goods(province_t *province)
{
    for (int i = 0; i < province->market.goods_count; ++i)
    {
        char *str = good_to_string(province->market.goods[i]);
        printf("%s\n", str);
        free(str);
    }
}

int main()
{
    market_good good = create_good("Grain", 5, FOOD);
    province_t province = create_province("capital");
    add_good(&province, good);
    for (int i = 0; i < 5; ++i)
    {
        process_local_market(&province);
        handle_population(&province, 0.5);
        print_goods(&province);
        printf("POPULATION: %f\n", province.population);
    }
}