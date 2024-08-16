import React, { PureComponent,useRef,useCallback,forwardRef,Component } from 'react';
import axios from "axios";
import ReactDOM from 'react-dom'
import Dropdown from 'react-bootstrap/Dropdown';
import { useState, useEffect } from "react";
import { EVENTS_LIST_URL, EVENTS_UPDATE_URL, NODES_LIST_URL } from "../apis/urls";
import { NODE_COUNT_URL, EVENTS_COUNT_URL, OS_COUNT_URL, SEVERITY_COUNT_URL, ALERT_COUNT_URL, HIGH_RISK_COUNT_URL,NODE_ALERT_COUNT_URL,WEEKLY_ALERTS_COUNT_URL} from "../apis/urls";
import { getDefaultHeaders, useFetchURL, useLazyFetch } from "../hooks";
import { Event,Node , toggle_state} from "../types";
import { Link } from "react-router-dom";
import { Navigate } from "react-router";
import { getGeoIp, Rapid_fetcher } from "../apis/requests";
import { timeConverter } from '../helpers';

import * as bootstrap from "bootstrap";
import swal from 'sweetalert'
import  loaderForTable from '../../../assets/images/loader-table.svg'
import { json } from "stream/consumers";
import ReactTooltip from "react-tooltip";
import Loader from '../components/loader';
// const WorldMap = require('react-world-map');
const GeoIP = () => {
  const [selected, onSelect] = useState(null);
  useEffect(() => {
    getGeoIp('182.57.16.34')
  },[])

  return (
    <>
      <div className="card card-body">
        <div className="hstack">

        </div>
      </div>
    </>
  );
};

export default GeoIP;