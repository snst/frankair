{
	"controls": {
		"aggregation": {
			"auto": true,
			"period": "1m"
		},
		"timespan": {
			"magnitude": "hour",
			"mode": "relative",
			"period": "latest",
			"value": 1
		}
	},
	"name": "frankair",
	"properties": {
		"columns": 6,
		"row_height": 40
	},
	"tabs": [
		{
			"icon": "fas fa-tachometer-alt",
			"name": "Main",
			"widgets": [
				{
					"layout": {
						"col": 2,
						"row": 0,
						"sizeX": 4,
						"sizeY": 5
					},
					"panel": {
						"color": "#ffffff",
						"colors": [],
						"title": "Temperature [°C]"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false
					},
					"sources": [
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "temp_exhaust_in",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#008a02",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside IN",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "temp_fresh_out",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#03e212",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside OUT",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "temp_fresh_in",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#0300b3",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Outside IN",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "temp_exhaust_out",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#8c8aff",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Outside OUT",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						}
					],
					"type": "chart"
				},
				{
					"layout": {
						"col": 2,
						"row": 10,
						"sizeX": 4,
						"sizeY": 5
					},
					"panel": {
						"color": "#ffffff",
						"title": "Abs humidity [g / mᵌ]"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false,
						"yaxis": {
							"min": 0
						}
					},
					"sources": [
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "humidity_abs_exhaust_in",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#180cc0",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_abs_exhaust_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "humidity_abs_fresh_out",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#4a9a28",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_abs_fresh_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Outside",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						}
					],
					"type": "chart"
				},
				{
					"layout": {
						"col": 2,
						"row": 20,
						"sizeX": 4,
						"sizeY": 6
					},
					"panel": {
						"color": "#ffffff",
						"title": "Rel humidity [%]"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false,
						"yaxis": {
							"max": 100,
							"min": 0
						}
					},
					"sources": [
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "humidity_rel_exhaust_in",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#146499",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_exhaust_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside IN",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "humidity_rel_fresh_out",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#378a14",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_fresh_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside OUT",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						}
					],
					"type": "chart"
				},
				{
					"layout": {
						"col": 2,
						"row": 15,
						"sizeX": 4,
						"sizeY": 5
					},
					"panel": {
						"color": "#ffffff",
						"title": "Actuator"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false,
						"yaxis": {
							"max": 10,
							"min": 0
						}
					},
					"sources": [
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "fan_level_exhaust",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#334552",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_exhaust",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Fan exhaust",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "fan_level_fresh",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#0592f0",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_fresh",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Fan fresh",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "fan_level_frost",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#ee692f",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_frost",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Fan frost",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "open_flap_frost",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#f5d924",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "open_flap_frost",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Flap frost open",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "mode",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#53bf45",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "mode",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Mode",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						},
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "submode",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#ef0bd0",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "submode",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "AutoMode",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 30
							}
						}
					],
					"type": "chart"
				},
				{
					"layout": {
						"col": 1,
						"row": 26,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffdca0",
						"subtitle": "",
						"title": "Outside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"iconSize": "",
						"max": 100,
						"min": 0,
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 28,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffdca0",
						"title": "Inside OUT"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 26,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffdca0",
						"title": "Inside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 28,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffdca0",
						"title": "Outside OUT"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 40,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Sum for"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"source": "code",
						"template": "<p>\n{{ ((value.running_sec / 86400) | number:0) }} days, \n{{ (((value.running_sec % 86400) / 3660) | number:0) }} hours, \n{{ ((value.running_sec % 3600) / 60) | number:0 }} min, \n{{ value.running_sec % 60 }} sec <br>\n( {{value.running_sec }} secs )\n</p>\n",
						"unit": "h",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": [
									"running_sec"
								],
								"resource": "Statistics",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "html"
				},
				{
					"layout": {
						"col": 1,
						"row": 43,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Fresh air sum"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "m³",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "volume_m3_sum",
								"resource": "Statistics",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 22,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#94dfff",
						"title": "Abs humidity inside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "g / mᵌ",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_abs_exhaust_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 22,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#94dfff",
						"title": "Abs humidity outside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "g / mᵌ",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_abs_fresh_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 24,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#e1effe",
						"title": "Rel humidity inside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "%",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_exhaust_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 24,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#e1effe",
						"title": "Rel humidity inside OUT"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "%",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_fresh_out",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 30,
						"sizeX": 1,
						"sizeY": 6
					},
					"panel": {
						"color": "#ffffff",
						"title": "Status"
					},
					"properties": {
						"source": "code",
						"template": "<div ng-app=\"\" ng-init=\"submode=['waiting','on','sniffing', 'undefined']; mode=['off','auto','manual', 'undefined']\">\n\n    <p>Mode : {{mode[value.mode] }}</p>\n    <p>AutoMode: {{submode[value.submode] }}</p>\n    <p>Fan off conditions: {{value.ctrl_active_humidity_fan_off}}</p>\n    <p>Humidity fan curve level: {{value.ctrl_active_humidity_fan_curve}}</p>\n    <p>Temp fan curve level: {{value.ctrl_active_temp_fan_curve}}</p>\n    <p>Frost fan curve level: {{value.ctrl_active_frost_fan_curve}}</p>\n\n</div>"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": [
									"ctrl_active_frost_fan_curve",
									"ctrl_active_humidity_fan_curve",
									"ctrl_active_humidity_fan_off",
									"ctrl_active_temp_fan_curve",
									"mode",
									"submode"
								],
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "html"
				},
				{
					"layout": {
						"col": 1,
						"row": 15,
						"sizeX": 1,
						"sizeY": 7
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Abs humidity delta (IN - OUT)"
					},
					"properties": {
						"colors": [
							{
								"color": "#809fff",
								"max": 0,
								"min": -10
							},
							{
								"color": "#a2ff70",
								"max": 10,
								"min": 0
							}
						],
						"majorTicks": 2,
						"max": 10,
						"min": -10,
						"plateColor": "#ffffff",
						"showValue": true,
						"textColor": "#1E313E",
						"tickColor": "#000000",
						"unit": "g/m³"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_abs_delta",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "tachometer"
				},
				{
					"layout": {
						"col": 0,
						"row": 15,
						"sizeX": 1,
						"sizeY": 7
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Rel humidity inside IN"
					},
					"properties": {
						"colors": [
							{
								"color": "#ffc8c8",
								"max": 40,
								"min": 0
							},
							{
								"color": "#86fe86",
								"max": 70,
								"min": 40
							},
							{
								"color": "#809fff",
								"max": 100,
								"min": 70
							}
						],
						"majorTicks": 10,
						"max": 100,
						"min": 0,
						"plateColor": "#ffffff",
						"showValue": true,
						"textColor": "#1E313E",
						"tickColor": "#000000",
						"unit": "%"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_exhaust_in",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "tachometer"
				},
				{
					"layout": {
						"col": 2,
						"row": 5,
						"sizeX": 4,
						"sizeY": 5
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Abs humidity delta [g / mᵌ]"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false,
						"yaxis": {
							"max": 10,
							"min": -10
						}
					},
					"sources": [
						{
							"bucket": {
								"backend": "dynamodb",
								"id": "Bucket",
								"mapping": "humidity_abs_delta",
								"tags": {},
								"user": "olavwaldherr"
							},
							"color": "#180cc0",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_abs_delta",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "inside - outside",
							"skipCurrent": 1,
							"source": "bucket",
							"timespan": {
								"magnitude": "minute",
								"mode": "configurable",
								"value": 10
							}
						}
					],
					"type": "chart"
				},
				{
					"layout": {
						"col": 0,
						"row": 38,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Water sum"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "l",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "liter_sum",
								"resource": "Statistics",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 36,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Water"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "l/h",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "liter_per_hour",
								"resource": "Statistics",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 41,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Fresh air"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "m³/h",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "volume_m3_per_hour",
								"resource": "Statistics",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 36,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"title": "Flap frost open"
					},
					"properties": {
						"color": "#1E313E",
						"gradient": true,
						"max": 10,
						"min": 0
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "open_flap_frost",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "gauge"
				},
				{
					"layout": {
						"col": 1,
						"row": 39,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Running for"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"source": "code",
						"template": "<p>\n{{ ((value.running_sec / 86400) | number:0) }} days, \n{{ (((value.running_sec % 86400) / 3660) | number:0) }} hours, \n{{ ((value.running_sec % 3600) / 60) | number:0 }} min, \n{{ value.running_sec % 60 }} sec <br>\n( {{value.running_sec }} secs )\n</p>\n",
						"unit": "h",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": [
									"running_sec"
								],
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "html"
				},
				{
					"layout": {
						"col": 1,
						"row": 33,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"title": "Fan frost level"
					},
					"properties": {
						"color": "#1E313E",
						"gradient": true,
						"max": 10,
						"min": 0
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_frost",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "gauge"
				},
				{
					"layout": {
						"col": 1,
						"row": 30,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"title": "Fan fresh level"
					},
					"properties": {
						"color": "#1E313E",
						"gradient": true,
						"max": 10,
						"min": 0
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_fresh",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "gauge"
				},
				{
					"layout": {
						"col": 0,
						"row": 42,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"title": "Efficiency"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit": "%",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "efficiency",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"panel": {
						"color": "#ffffff",
						"title": "Frankair"
					},
					"type": "html",
					"sources": [
						{
							"name": "Default",
							"color": "#1E313E",
							"source": "device",
							"device": {
								"id": "devid1",
								"user": "olavwaldherr",
								"resource": "State",
								"update": "interval",
								"interval": 5,
								"mapping": [
									"efficiency",
									"fan_level_exhaust",
									"fan_level_fresh",
									"fan_level_frost",
									"humidity_abs_delta",
									"humidity_abs_exhaust_in",
									"humidity_abs_fresh_out",
									"humidity_rel_exhaust_in",
									"humidity_rel_fresh_out",
									"liter_per_hour",
									"open_flap_frost",
									"temp_exhaust_in",
									"temp_fresh_in",
									"temp_exhaust_out",
									"temp_fresh_out",
									"volume_m3_per_hour",
									"mode",
									"submode"
								]
							},
							"skipCurrent": 1
						}
					],
					"properties": {
						"source": "code",
						"template": "<style>\n\n.image-container {\n  position: relative; /* allow absolute positioning of text within the div */\n  width: 78%; /* fill the width of the parent container */\n  padding-bottom: 100%; /* maintain aspect ratio of the image (width / height = 100 / 75) */\n  background-image: url('https://raw.githubusercontent.com/snst/frankair/main/dashboard/main/bg.jpeg');\n  background-size: cover; /* keep the aspect ratio of the image */\n  background-repeat: no-repeat;\n  background-position: center;\n}\n.valh {\n  color: black;\n}\n\n.val {\n  background-color: white;\n  font-size: 1.5em;\n  padding: 5px 10px 5px;\n}\n\n.image-container .frost {\n  position: absolute;\n  top: 5%; \n  left: 50%;\n   transform: translate(-50%, 0%);\n}\n\n.image-container .fan_exhaust {\n  position: absolute;\n  top: 32%; \n  left: 20%;\n   transform: translate(-50%, 0%);\n}\n\n.image-container .fan_fresh {\n  position: absolute;\n  top: 32%; \n  left: 75%;\n   transform: translate(-50%, 0%);\n}\n\n.image-container .inside_in {\n  position: absolute;\n  top: 5%; \n  left: 15%;\n   transform: translate(-50%, 0%);\n}\n\n.image-container .outside_in {\n  position: absolute;\n  top: 5%; \n  left: 80%;\n   transform: translate(-50%, 0%);\n}\n\n.image-container .inside_out {\n  position: absolute;\n  top: 80%; \n  left: 15%;\n   transform: translate(-50%, 0%);\n}\n\n.image-container .outside_out {\n  position: absolute;\n  top: 80%; \n  left: 80%;\n   transform: translate(-50%, 0%);\n}\n\n.image-container .hum {\n  position: absolute;\n  top: 45%; \n  left: 50%;\n   transform: translate(-50%, 0%);\n}\n\n\n</style>\n\n<div class=\"image-container\">\n\n<div class=\"val hum\">\n    Hum delta: {{value.humidity_abs_delta | number:2}}g/m³<br>\n    Water: {{value.liter_per_hour | number:2}} l/h<br>\n    Air: {{value.volume_m3_per_hour | number:2}} m³/h<br>\n    Efficiency: {{value.efficiency | number:0}} %<br>\n    \n</div>\n\n\n<div class=\"val fan_exhaust\">\n    Exhaust: {{value.fan_level_exhaust}}<br>\n</div>\n\n<div class=\"val fan_fresh\">\n    Fresh: {{value.fan_level_fresh}}\n</div>\n\n<div class=\"val frost\">\n    <div class=\"valh\">Frost</div>\n    Open: {{value.open_flap_frost}}<br>\n    Fan: {{value.fan_level_frost}}\n</div>\n\n<div class=\"val inside_in\">\n    <div class=\"valh\">Inside_IN</div>\n    {{value.temp_exhaust_in | number:1}} °C<br>\n    {{value.humidity_abs_exhaust_in | number:1}} g/m³<br>\n    {{value.humidity_rel_exhaust_in | number:1}} %<br>\n</div>\n\n<div class=\"val outside_in\">\n    <div class=\"valh\">Outside_IN</div>\n    {{value.temp_fresh_in | number:1}} °C<br>\n    {{value.humidity_abs_fresh_out | number:1}} g/m³<br>\n</div>\n\n<div class=\"val inside_out\">\n    <div class=\"valh\">Inside_OUT</div>\n    {{value.temp_fresh_out | number:1}} °C<br>\n    {{value.humidity_rel_fresh_out | number:1}} %<br>\n</div>\n\n<div class=\"val outside_out\">\n    <div class=\"valh\">Outside_OUT</div>\n    {{value.temp_exhaust_out | number:1}} °C<br>\n</div>\n\n\n</div>\n\n"
					},
					"layout": {
						"sizeX": 2,
						"sizeY": 15,
						"row": 0,
						"col": 0
					}
				}
			]
		},
		{
			"name": "History",
			"icon": "fas fa-atom",
			"widgets": []
		},
		{
			"name": "Digital",
			"icon": "fas fa-atom",
			"widgets": []
		},
		{
			"icon": "fas fa-atom",
			"name": "Raw",
			"widgets": [
				{
					"layout": {
						"col": 0,
						"row": 6,
						"sizeX": 6,
						"sizeY": 7
					},
					"panel": {
						"color": "#ffffff",
						"title": "Temperature [°C]"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false
					},
					"sources": [
						{
							"color": "#ff0000",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_in",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside IN",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 5
							}
						},
						{
							"color": "#9a6a19",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_out",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Outside OUT",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 5
							}
						},
						{
							"color": "#0099ff",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_in",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Outside IN",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 5
							}
						},
						{
							"color": "#3bc931",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_out",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside OUT",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 5
							}
						}
					],
					"type": "chart"
				},
				{
					"layout": {
						"col": 0,
						"row": 13,
						"sizeX": 6,
						"sizeY": 8
					},
					"panel": {
						"color": "#ffffff",
						"title": "Relative humidity [%]"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false
					},
					"sources": [
						{
							"color": "#0573bd",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_fresh_out",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside OUT",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 5
							}
						},
						{
							"color": "#3e211e",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_exhaust_in",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Inside IN",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 5
							}
						}
					],
					"type": "chart"
				},
				{
					"layout": {
						"col": 1,
						"row": 0,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Temperature inside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_in",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 2,
						"row": 3,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Temperature outside OUT"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_exhaust_out",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 3,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Rel humidity inside OUT"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "%",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_fresh_out",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 0,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Rel humidity inside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "%",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "humidity_rel_exhaust_in",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 2,
						"row": 0,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Temperature outside IN"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_in",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 3,
						"sizeX": 1,
						"sizeY": 3
					},
					"panel": {
						"color": "#ffffff",
						"subtitle": "",
						"title": "Temperature inside OUT"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 2,
						"icon": "",
						"size": "35px",
						"unit": "°C",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "temp_fresh_out",
								"resource": "StateRAW",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 3,
						"row": 0,
						"sizeX": 3,
						"sizeY": 6
					},
					"panel": {
						"color": "#ffffff",
						"title": "Actuator"
					},
					"properties": {
						"axis": true,
						"fill": false,
						"legend": true,
						"multiple_axes": false,
						"yaxis": {
							"max": 10,
							"min": 0
						}
					},
					"sources": [
						{
							"color": "#3bd204",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_fresh",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Fresh fan level",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 30
							}
						},
						{
							"color": "#db720f",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_exhaust",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Exhaust fan level",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 30
							}
						},
						{
							"color": "#0292f2",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "fan_level_frost",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Frost fan level",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 30
							}
						},
						{
							"color": "#003a61",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "open_flap_frost",
								"resource": "State",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Frost flap open",
							"skipCurrent": 1,
							"source": "device",
							"timespan": {
								"magnitude": "minute",
								"value": 30
							}
						}
					],
					"type": "chart"
				}
			]
		},
		{
			"icon": "fas fa-atom",
			"name": "OTA",
			"widgets": [
				{
					"layout": {
						"col": 1,
						"row": 2,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Transferred size"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "transferted_size",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 6,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Error"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "error",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 2,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Firmware size"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "firmware_size",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 4,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "HTTP response"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "http_response",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 2,
						"row": 4,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Simulate"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "simulate",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 6,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Success"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "success",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 2,
						"row": 2,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Checksum"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "checksum",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 0,
						"row": 0,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Version"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "version",
								"resource": "Info",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 2,
						"row": 6,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Abort"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "abort",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 4,
						"sizeX": 1,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Downloading"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "downloading",
								"resource": "OTA",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				},
				{
					"layout": {
						"col": 1,
						"row": 0,
						"sizeX": 2,
						"sizeY": 2
					},
					"panel": {
						"color": "#ffffff",
						"title": "Build"
					},
					"properties": {
						"color": "#1E313E",
						"decimal_places": 0,
						"icon": "",
						"size": "35px",
						"unit_size": "20px",
						"weight": "font-thin"
					},
					"sources": [
						{
							"color": "#1E313E",
							"device": {
								"id": "devid1",
								"interval": 5,
								"mapping": "build",
								"resource": "Info",
								"update": "interval",
								"user": "olavwaldherr"
							},
							"name": "Default",
							"skipCurrent": 1,
							"source": "device"
						}
					],
					"type": "text"
				}
			]
		}
	]
}