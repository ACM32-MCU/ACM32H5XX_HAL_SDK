#include "bsp_lan8720.h" 

HAL_StatusTypeDef bsp_lan8720_init(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
    
	return (bsp_lan8720_soft_reset(heth));
}

HAL_StatusTypeDef bsp_lan8720_deinit(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
    
    return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_soft_reset(ETH_HandleTypeDef *heth)
{
	uint32_t tick_start;
	uint32_t value;
	
	if((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, LAN8720_BCR_RESET)) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	tick_start = HAL_GetTick();
	do
	{
		if ((HAL_GetTick() - tick_start ) > LAN8720_TIMEOUT_RESET)
		{
			return (HAL_ERROR);
		}

		if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
		{
			return (HAL_ERROR);
		}

	} while (((value & LAN8720_BCR_RESET) == LAN8720_BCR_RESET));
	
	return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_power_down_enable(ETH_HandleTypeDef *heth)
{
	uint32_t value;
	
	// Disable: AutoNegotiation
	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	if ((value & LAN8720_BCR_AUTO_NEGOTIATION) == LAN8720_BCR_AUTO_NEGOTIATION)
	{
		value &= ~LAN8720_BCR_AUTO_NEGOTIATION;
		
		if((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
		{
			return (HAL_ERROR);
		}
	}
	
	value |= LAN8720_BCR_POWER_DOWN;
	
	if((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_power_down_disable(ETH_HandleTypeDef *heth)
{
	uint32_t value;
	
	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	value &= ~LAN8720_BCR_POWER_DOWN;
	
	if((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_auto_negotiation_enable(ETH_HandleTypeDef *heth)
{
	uint32_t tick_start;
	uint32_t value;

	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}

	// Turn off POWER DOWN mode.
	if ((value & LAN8720_BCR_POWER_DOWN) == LAN8720_BCR_POWER_DOWN)
	{
		// If it is in POWER DOWN mode, POWER DOWN mode is turned off.
		value &= ~LAN8720_BCR_POWER_DOWN;
		
		if ((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, LAN8720_BCR_AUTO_NEGOTIATION)) != HAL_OK)
		{
			return (HAL_ERROR);
		}
		
		// Software reset
		bsp_lan8720_soft_reset(heth);
		
		if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
		{
			return (HAL_ERROR);
		}
	}
	
	value |= LAN8720_BCR_AUTO_NEGOTIATION;
		
	if ((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
	{
		return (HAL_ERROR);
	}

	tick_start = HAL_GetTick();

	do
	{
		if ((HAL_GetTick() - tick_start ) > LAN8720_TIMEOUT_AUTO_NEGOTIATION)
		{
			return (HAL_ERROR);
		}
		
		if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_PSCSR, &value) != HAL_OK)
		{
			return (HAL_ERROR);
		}

	} while (((value & LAN8720_PSCSR_AUTO_NEGOTIATION_STATUS) != LAN8720_PSCSR_AUTO_NEGOTIATION_STATUS));

	return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_auto_negotiation_disable(ETH_HandleTypeDef *heth)
{
	uint32_t value;

	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}

	if ((value & LAN8720_BCR_AUTO_NEGOTIATION) == 0)
	{
		return (HAL_OK);
	}
	
	value &= ~LAN8720_BCR_AUTO_NEGOTIATION;
		
	if ((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_get_link_state(ETH_HandleTypeDef *heth, uint32_t *auto_negotiation, uint32_t *state)
{
	uint32_t value;
	
	if ((heth == NULL) || (auto_negotiation == NULL) || (state == NULL))
	{
		return (HAL_ERROR);
	}
	
	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BSR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}

	if ((value & LAN8720_BSR_LINK_STATUS) == 0)
	{
		*state = LAN8720_LINK_DOWN;
		
		if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
		{
			return (HAL_ERROR);
		}
	
		if ((value & LAN8720_BCR_AUTO_NEGOTIATION) == 0)
			*auto_negotiation = LAN8720_AUTO_NEGOTIATION_DISABLE;
		else
			*auto_negotiation = LAN8720_AUTO_NEGOTIATION_ENABLE;
		
		return (HAL_OK);
	}
	
	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}

	if ((value & LAN8720_BCR_AUTO_NEGOTIATION) != 0)
	{
		if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BSR, &value) != HAL_OK)
		{
			return (HAL_ERROR);
		}
		
		if ((value & LAN8720_BSR_AUTO_NEGOTIATION_COMPLETE) == 0)
		{
			*state = LAN8720_LINK_AUTO_NEGOTIATION;
			return (HAL_OK);
		}
		
		if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_PSCSR, &value) != HAL_OK)
		{
			return (HAL_ERROR);
		}
		
		value &= LAN8720_PSCSR_SPEED_STATUS;
		
		if (value == LAN8720_PSCSR_SPEED_STATUS_10M_HALF_DUPLEX)
		{
			*state = LAN8720_LINK_10M_HALF_DUPLEX;
		}
		else if (value == LAN8720_PSCSR_SPEED_STATUS_10M_FULL_DUPLEX)
		{
			*state = LAN8720_LINK_10M_FULL_DUPLEX;
		}
		else if (value == LAN8720_PSCSR_SPEED_STATUS_100M_HALF_DUPLEX)
		{
			*state = LAN8720_LINK_100M_HALF_DUPLEX;
		}
		else if (value == LAN8720_PSCSR_SPEED_STATUS_100M_FULL_DUPLEX)
		{
			*state = LAN8720_LINK_100M_FULL_DUPLEX;
		}
		else
		{
			return (HAL_ERROR);
		}
		return (HAL_OK);
	}
	else
	{
		if ((value & LAN8720_BCR_SPEED) == 0)
		{
			if ((value & LAN8720_BCR_DUPLEX_MODE) == 0)
			{
				*state = LAN8720_LINK_10M_HALF_DUPLEX;
			}
			else
			{
				*state = LAN8720_LINK_10M_FULL_DUPLEX;
			}
		}
		else
		{
			if ((value & LAN8720_BCR_DUPLEX_MODE) == 0)
			{
				*state = LAN8720_LINK_100M_HALF_DUPLEX;
			}
			else
			{
				*state = LAN8720_LINK_100M_FULL_DUPLEX;
			}
		}
		return (HAL_OK);
	}
}

HAL_StatusTypeDef bsp_lan8720_set_link_state(ETH_HandleTypeDef *heth, uint32_t auto_negotiation, uint32_t state)
{
	uint32_t value;
	
	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	if (auto_negotiation != LAN8720_AUTO_NEGOTIATION_DISABLE)
	{
		value |= LAN8720_BCR_AUTO_NEGOTIATION;
		
		if ((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
		{
			return (HAL_ERROR);
		}
	}
	else
	{
		value &= ~(LAN8720_BCR_AUTO_NEGOTIATION | LAN8720_BCR_SPEED | LAN8720_BCR_DUPLEX_MODE);
		
		if (state == LAN8720_LINK_10M_HALF_DUPLEX)
		{
		}
		else if (state == LAN8720_LINK_10M_FULL_DUPLEX)
		{
			value |= LAN8720_BCR_DUPLEX_MODE;
		}
		else if (state == LAN8720_LINK_100M_HALF_DUPLEX)
		{
			value |= LAN8720_BCR_SPEED;
		}
		else if (state == LAN8720_LINK_100M_FULL_DUPLEX)
		{
			value |= LAN8720_BCR_SPEED | LAN8720_BCR_DUPLEX_MODE;
		}
		else
		{
			return (HAL_ERROR);
		}
		
		if ((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
		{
			return (HAL_ERROR);
		}
	}
	
	return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_loop_back_enable(ETH_HandleTypeDef *heth)
{
	uint32_t value;
	
	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	if ((value & LAN8720_BCR_LOOP_BACK) == LAN8720_BCR_LOOP_BACK)
	{
		return (HAL_OK);
	}
	
	value |= LAN8720_BCR_LOOP_BACK;
	
	if ((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	return (HAL_OK);
}

HAL_StatusTypeDef bsp_lan8720_loop_back_disable(ETH_HandleTypeDef *heth)
{
	uint32_t value;
	
	if (HAL_ETH_ReadPHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, &value) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	if ((value & LAN8720_BCR_LOOP_BACK) == 0)
	{
		return (HAL_OK);
	}
	
	value &= ~LAN8720_BCR_LOOP_BACK;
	
	if ((HAL_ETH_WritePHYRegister(heth, heth->Init.PhyAddress, LAN8720_REG_BCR, value)) != HAL_OK)
	{
		return (HAL_ERROR);
	}
	
	return (HAL_OK);
}
