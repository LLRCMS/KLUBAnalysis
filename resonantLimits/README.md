1) make_res_cards.sh:

   Calls write_res_card.py, generates the datacards per channel/category/mass point. ABCD regions for QCD estimate generated separately

2) make_workspace_res.sh:

   Combines ABCD regions datacards, and generates workspaces for each channel/category/mass point

	 2-1) combine_res_categories.sh

	      Combines datacards from the 4 categories, and generates workspaces for each channel/mass point

	 2-2) combine_res_channels.sh

	      Combines datacards from the 3 channels, and generates workspaces for each category/mass point

	 2-3) combine_res_all.sh
	      
	      Combines all datacards for the period, and generates workspaces for each mass point

3) get_limits_res.sh:

   Runs combine for asymptotic limits for all channel/category/mass point separately, stores result in a log file for easy limit plotting.

4) plotSimple_resMass.py
