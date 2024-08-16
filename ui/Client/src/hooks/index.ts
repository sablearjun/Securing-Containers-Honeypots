import { useCallback, useEffect, useState,useRef } from "react";
//import { TOKEN } from "../apis/base-urls";
import axios from "axios";
import { table } from "console";
import Alerts from "../pages/alerts";
import { REGISTER_GET_TOKEN_URL } from "../apis/urls";
import { LOGIN_GET_TOKEN_URL } from "../apis/urls";
import { useIsomorphicLayoutEffect } from 'usehooks-ts'

export const getDefaultHeaders = (init?: HeadersInit) =>
  new Headers({
    Authorization: "Token " + localStorage.getItem('Token'),
    Accept: "application/json",
    ...init,
  });
//for Register
export function useRegisterUser<T = Record<string, string>>() {
  const [{ loading, error, data }, setState] = useState<{
    loading: boolean;
    data: T | null;
    error: Error | null;
  }>({
    loading: false,
    data: null,
    error: null,
  });

  const fetchAndUpdateState = useCallback(async function fetchAndUpdateState({
    username,
    password,
    email,
    firstname,
    lastname,
  }: {
    username: string;
    password: string;
    email: string;
    firstname: string;
    lastname: string;
  }) {
    // console.log("Inside register");
    const response = fetch(REGISTER_GET_TOKEN_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        username,
        password,
        email,
        first_name: firstname,
        last_name: lastname,
      }),
    });
    setState({ loading: true, data: null, error: null });
    try {
      let response = await fetch(REGISTER_GET_TOKEN_URL, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          username,
          password,
          email,
          first_name: firstname,
          last_name: lastname,
        }),
      });

      if (response.ok) {
        let data = await response.json();
        // console.log("Register Working");
        // console.log(data);
        setState({ loading: false, data, error: null });
      } else {
        setState({
          loading: false,
          data: null,
          error: new Error(response.statusText),
        });
      }
    } catch (e) {
      setState({
        loading: false,
        data: null,
        error: e as Error,
      });
    }
  },
  []);

  return { loading, error, data, run: fetchAndUpdateState };
}

// for alert table,latest Alerts,update alerts
export function useFetchURL<T = Record<string, string>>({
  URL,
  init,
}: {
  URL: string;
  init?: RequestInit;
}) {
  const [{ loading, error, data }, setState] = useState<{
    loading: boolean;
    data: T | null;
    error: Error | null;
  }>({
    loading: false,
    data: null,
    error: null,
  });

  const [queryProps] = useState({ URL, init });
  //console.log(queryProps);
  const fetchAndUpdateState = useCallback(
    async function fetchAndUpdateState() {
      // a.forEach(every=>{
      //   console.log('Tejal')
      //   console.log(every)
      // })

      setState({ loading: true, data: null, error: null });
      try {
        let response = await fetch(queryProps.URL, {
          method: "GET",
          headers: getDefaultHeaders(),
          // body:xyz(),
          ...queryProps.init,
        });

        if (response.ok) {
          let data = await response.json();
          // console.log(data);
          //console.log(queryProps.URL);
          setState({ loading: false, data, error: null });
        } else {
          setState({
            loading: false,
            data: null,
            error: new Error(response.statusText),
          });
        }
      } catch (e) {
        setState({
          loading: false,
          data: null,
          error: e as Error,
        });
      }
    },
    [queryProps]
  );

  useEffect(() => {
    fetchAndUpdateState();
  }, [fetchAndUpdateState]);

  return { loading, error, data, refetch: fetchAndUpdateState };
}

export function useLazyFetch<T = Record<string, string>>() {
  const [{ loading, error, data }, setState] = useState<{
    loading: boolean;
    data: T | null;
    error: Error | null;
  }>({
    loading: false,
    data: null,
    error: null,
  });

  const fetchAndUpdateState = useCallback(async function fetchAndUpdateState({
    URL,
    init,
  }: {
    URL: string;
    init?: RequestInit;
  }) {
    // console.log(`Inside run ${URL}`);

    setState({ loading: true, data: null, error: null });
    try {
      let response = await fetch(URL, {
        method: "GET",
        headers: getDefaultHeaders(),
        ...init,
      });

      if (response.ok) {
        let data = await response.json();
        // console.log(data);
        setState({ loading: false, data, error: null });
      } else {
        setState({
          loading: false,
          data: null,
          error: new Error(response.statusText),
        });
      }
    } catch (e) {
      setState({
        loading: false,
        data: null,
        error: e as Error,
      });
    }
  },
  []);

  return { loading, error, data, run: fetchAndUpdateState };
}


function useInterval(callback: () => void, delay: number | null) {
  const savedCallback = useRef(callback)

  // Remember the latest callback if it changes.
  useIsomorphicLayoutEffect(() => {
    savedCallback.current = callback
  }, [callback])

  // Set up the interval.
  useEffect(() => {
    // Don't schedule if no delay is specified.
    // Note: 0 is a valid value for delay.
    if (!delay && delay !== 0) {
      return
    }

    const id = setInterval(() => savedCallback.current(), delay)

    return () => clearInterval(id)
  }, [delay])
}

export default useInterval