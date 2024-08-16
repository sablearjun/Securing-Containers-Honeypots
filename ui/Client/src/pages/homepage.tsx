import React, { FormEvent, useEffect, useLayoutEffect, useState } from "react";
import { useNavigate, useLocation } from "react-router-dom";
import Loader from "../components/loader";
import { NavLink } from "react-router-dom";
import ControlledCarousel from "../components/carousel";
import Logo from "../assets/images/vajralogo.svg";
import mainpagedashboard from "../assets/images/mainpagedashboard.png";
import iitblogo from "../assets/images/iitblogo.png";
import trustlablogo from "../assets/images/trustlab.png";
import ncetislogo from "../assets/images/ncetislogo.jpg";
import tcaaiLogo from "../assets/images/tcaaiLogo.png";
import vajraedr from "../assets/images/vajraedr.png";
import arjunSablePic from "../assets/images/arjun_sable.jpg";
import atulKabraSirPic from "../assets/images/atul_kabra_sir.jpg";
import bhumikaKothwalPic from "../assets/images/bhumika_kothwal.jpg";
import deveshSawantPic from "../assets/images/devesh_sawant.jpg";
import gauriShewalePic from "../assets/images/gauri_shewale.jpg";
import manjeshHanawalSirPic from "../assets/images/manjesh_hanawal_sir.jpg";
import sameerKulkarniSirPic from "../assets/images/sameer_kulkarni_sir.jpg";
import yogeshJadhavPic from "../assets/images/yogesh_jadhav.jpg";
import HomePageNavbar from "../components/navbar/homepageNavbar";
import BlogList from "../components/blog/BlogList";

const HomePage = () => {
  let navigate = useNavigate();
  const location = useLocation();

  const [isloading, setIsLoading] = useState(false);

  useLayoutEffect(() => {
    try {
      const authenticatedUser = sessionStorage.getItem("authenticatedUser");
      if (authenticatedUser) {
        const { isAuthenticated } = JSON.parse(authenticatedUser);

        if (isAuthenticated) {
          window.location.href = "/node";
        }
      }
    } catch (e) {}
  }, [navigate]);

  return isloading ? (
    <Loader />
  ) : (
    <div className="vh-100">
      {/* Navbar */}
      <div className="d-block min-vh-auto">
        <HomePageNavbar/>
      </div>
      {/* Banner */}
      <div>
        <div className="d-flex flex-wrap justify-content-center align-items-center gap-1 bg-login vh-25">
          <div className="row mx-7">
            <h1 className="fw-light text-primary outfit fs-1 d-flex justify-content-center text-center flex-column flex-md-row">
              <span className="fw-mid fs-1">Vajra:&nbsp;
              <span className="fw-light p-0 m-0">
                An Indigenous Solution for Endpoint <span className="d-none d-md-block">{"\n"}</span>Detection and Management
              </span>
              </span>
            </h1>
          </div>
        </div>
      </div>

      {/*  About Section */}
      <div>
        <div
          id="about"
          className="d-flex flex-wrap justify-content-center align-items-center gap-8 h-100"
        >
          <div className="row m-0 p-1">
            <div>
              <h1 className="fw-bold text-primary outfit fs-1 text-start my-0 my-md-3 ps-4">
                About Vajra
              </h1>
            </div>
            <div className="row m-0">
              <div className="col-12 col-md-6">
                <div className="d-flex flex-column justify-content-start mx-3 my-4">
                  <p className="fw-mid text-black-50 text-justify fs-4 lh-base mt-1">
                    According to the fables in Indian mythologies,&nbsp;
                    <a href="https://en.wikipedia.org/wiki/Vajra">Vajra</a> is
                    one of the most powerful weapons in the entire universe and
                    is used to defend against the forces intending to attack the
                    institutions of 'dharma'. Project Vajra, an indigenously
                    developed endpoint security system at IIT Bombay (an
                    institute of national importance), embodies the
                    characteristics of the celestial vajra to provide a set of
                    high-quality defensive and offensive capabilities against
                    the growing and ever-changing landscape of cyber threats in
                    the country. Project Vajra is not intended to replace the
                    commercially available, sophisticated EDR solutions for
                    large-scale enterprise deployment. It, however, aims to
                    provide an alternative for micro, small, and medium-sized
                    businesses to improve their cyber security postures and
                    improve cyber security inclusion in the country. At the same
                    time, given its indigenous and homegrown nature, it can also
                    be another tool of defense in the arsenal of public
                    institutions, so data storage as well as processing remain
                    well within the confines of the national boundary. Digital
                    public goods and projects like UPI and India stack have
                    paved a path for homegrown deep tech innovation that the
                    rest of the world is emulating. With Vajra, the research
                    group at IIT Bombay aims to set a tone for pushing the
                    envelope of innovation in high tech domains like cyber
                    security so as to have easier and safer adoption of other
                    digital projects.
                  </p>
                </div>
              </div>
              <div className="col-12 col-md-6">
                <div className="d-flex flex-column justify-content-between align-items-start mx-3 my-4">
                  <ControlledCarousel />
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      {/* Features Section 1 */}
      <div>
        <div
          id="features"
          className="d-flex flex-wrap justify-content-center align-items-center gap-8 h-100"
        >
          <div className="container">
            <br />
            <br />
            <h1 className="fw-bold text-primary text-center outfit fs-1 mt-3">Features</h1>
            <div className="d-flex justify-content-start gap-7 my-7">
              <div className="d-flex flex-column justify-content-between align-items-start">
                <p className="fw-mid text-black-50 fs-4 lh-base mt-1 p-4 p-md-0">
                  Given that cyber attackers are constantly innovating novel
                  methods, an effective endpoint defence can only be achieved if
                  the solution has 'visibility, extensibility, and openness' at
                  its core.
                </p>
                <div className="d-flex flex-column flex-lg-row justify-content-between">
                  <div className="card flex-1 my-4 p-7 mx-3">
                    <h2 className="text-primary fs-4 lh-base mt-1">
                      <span>
                        <i className="fa fa-eye ms-0 me-2 text-primary"></i>
                      </span>
                      Visibility
                    </h2>
                    <p className="fw-mid text-black-50 fs-4 lh-base text-justify mt-1">
                      With a highly customizable and light-weight agent, Vajra
                      collects and analyses vast amounts of data from endpoints,
                      such as system logs, network traffic, process information,
                      and file behaviour. With high visibility, security teams
                      can detect and respond to potential threats more
                      effectively, identifying malicious activities, anomalous
                      behaviour, or indicators of compromise (IoCs). It allows
                      organisations to gain a deeper understanding of their
                      environment, identify vulnerabilities, and implement
                      proactive security measures.
                    </p>
                  </div>
                  <div className="card flex-1 my-4 p-7 mx-3">
                    <h2 className="fw-bold text-primary fs-4 lh-base mt-1">
                      <span>
                        <i className="fa fa-star ms-0 me-2 text-primary"></i>
                      </span>
                      Extensibitlity
                    </h2>
                    <p className="fw-mid text-black-50 fs-4 lh-base mt-1 text-justify">
                      When it comes to endpoint security, organisations require
                      the solution to align with their unique environment,
                      security policies, and threat landscape. Vajra enables the
                      creation of custom detection rules, automated response
                      actions, and integration with internal processes or
                      workflows. This flexibility ensures that Vajra can adapt
                      and evolve as security requirements change over time.
                    </p>
                  </div>
                  <div className="card flex-1 my-4 p-7 mx-3">
                    <h2 className="fw-bold text-primary fs-4 lh-base mt-1">
                      <span>
                        <i className="fa fa-users ms-0 me-2 text-primary"></i>
                      </span>
                      Openness
                    </h2>
                    <p className="fw-mid text-black-50 fs-4 lh-base mt-1 text-justify">
                      Vajra uses tried, tested, and trusted open source
                      components like osquery. This enables transparency in the
                      solution and also lays the foundation for building a
                      community that fosters collaboration because cyber
                      security is nothing short of a 'team sport'. Effective
                      sharing of data, alerts, and contextual information
                      between different organizations and security tools
                      enhances overall security posture and response
                      capabilities.
                    </p>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

    
      {/* Team */}
      <div>
        <div
          id="team"
          className="d-flex flex-wrap justify-content-center align-items-center gap-8 h-100"
        >
          <div className="container">
          <br />
            <br />
            <h1 className="fw-bold text-primary outfit fs-1 text-center mt-3">
              The Team
            </h1>
            <div className="row justify-content-center gap-7 my-7">
              <div className="col-12 col-md-3">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={manjeshHanawalSirPic}
                    alt="Manjesh Sir"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://www.ieor.iitb.ac.in/mhanawal"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Team Lead</h4>
                      Prof. Manjesh Kumar Hanawal
                    </a>
                  </div>
                </div>
              </div>
              <div className="col-12 col-md-3">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={atulKabraSirPic}
                    alt="Atul Sir"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://www.linkedin.com/in/atulkabra/"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Advisor</h4>
                      Mr. Atul Kabra
                    </a>
                  </div>
                </div>
              </div>
              <div className="col-12 col-md-3">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={sameerKulkarniSirPic}
                    alt="Sameer Sir"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://iitgn.ac.in/faculty/cse/sameer"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Collaborator</h4>
                      Prof. Sameer Kulkarni
                    </a>
                  </div>
                </div>
              </div>
            </div>
            {/* -- Team Members -- */}
            <div className="row justify-content-center my-md-3">
              {/* Arjun Sable */}
              <div className="col-12 col-lg-2 col-md-4 p-md-3 mx-md-4">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={arjunSablePic}
                    alt="Arjun Sable"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://www.linkedin.com/in/sablearjun/"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Senior Project Associate</h4>
                      Mr. Arjun Sable
                    </a>
                  </div>
                </div>
              </div>

              {/* Devesh Sawant */}
              <div className="col-12 col-lg-2 col-md-4 p-md-3 mx-md-4">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={deveshSawantPic}
                    alt="Devesh Sawant"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://www.linkedin.com/in/devesh-sawant-915534161/"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Core Engineer</h4>
                      Mr. Devesh Sawant
                    </a>
                  </div>
                </div>
              </div>

              {/* Yogesh Jadhav */}
              <div className="col-12 col-lg-2 col-md-4 p-md-3 mx-md-4">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={yogeshJadhavPic}
                    alt="Yogesh Jadhav"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://www.linkedin.com/in/yogeshjadhaav/"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Core Engineer</h4>
                      Mr. Yogesh Jadhav
                    </a>
                  </div>
                </div>
              </div>

              {/* Gauri Shewale */}
              <div className="col-12 col-lg-2 col-md-4 p-md-3 mx-md-4">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={gauriShewalePic}
                    alt="Gauri Shewale"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://www.linkedin.com/in/gauri-shewale-bb8040200"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Core Engineer</h4>
                      Ms. Gauri Shewale
                    </a>
                  </div>
                </div>
              </div>

              {/* Bhumika Kothwal */}
              <div className="col-12 col-lg-2 col-md-4 p-md-3 mx-md-4">
                <div className="d-flex flex-column justify-content-center align-items-center mx-3 my-4">
                  <img
                    src={bhumikaKothwalPic}
                    alt="Bhumika Kothwal"
                    width="150px"
                    className="rounded-circle"
                  />
                  <div className="text-size-small my-4 text-center">
                    <a
                      href="https://www.linkedin.com/in/bhumika-kothwal-3050a419b"
                      className="noUnderline fs-4 lh-base"
                    >
                      <h4 className="h4">Core Engineer</h4>
                      Ms. Bhumika Kothwal
                    </a>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      {/* Sponsoring Institutions */}
      <div>
        <div>
          <div className="container">
            <h1 className="fw-bold text-primary outfit fs-1 text-center my-8">
              Sponsoring Institutions
            </h1>
            <div className="row">
              <div className="col-12 col-md-4">
                <div className="d-flex flex-column justify-content-end align-items-center mx-3 my-4">
                  <img
                    src={tcaaiLogo}
                    alt="TCAAI Logo"
                    width="200px"
                    height="150px"
                  />
                  <div className="text-size-small my-2 text-center">
                    <a
                      href="https://www.tcaai.iitb.ac.in/"
                      className="h4 noUnderline fs-4 lh-base"
                      target="_blank"
                    >
                      TCAAI
                    </a>
                  </div>
                </div>
              </div>
              <div className="col-12 col-md-4">
                <div className="d-flex flex-column justify-content-end align-items-center mx-3 my-4">
                  <img
                    src={ncetislogo}
                    alt="NCETIS logo"
                    width="200px"
                    height="150px"
                  />
                  <div className="text-size-small my-2 text-center">
                    <a
                      href="http://www.ee.iitb.ac.in/~ncetis/"
                      className="h4 noUnderline fs-4 lh-base"
                      target="_blank"
                    >
                      NCETIS
                    </a>
                  </div>
                </div>
              </div>
              <div className="col-12 col-md-4">
                <div className="d-flex flex-column justify-content-end align-items-center mx-3 my-4">
                  <img
                    src={trustlablogo}
                    alt="Trust Lab Logo"
                    width="200px"
                    height="150px"
                  />
                  <div className="text-size-small my-2 text-center">
                    <a
                      href="https://trustlab.iitb.ac.in/"
                      className="h4 noUnderline fs-4 lh-base"
                      target="_blank"
                    >
                      Trust Lab
                    </a>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <br />
      <br />

      {/* Footer */}
      <div className="d-flex flex-wrap justify-content-center align-items-center gap-8 h-50 bg-login">
        <div className="container">
          <div className="row">
            <div className="col-12 col-md-6 mb-md-8">
              <div className="d-flex flex-column justify-content-start align-items-center mx-3 my-4">
                <img src={vajraedr} alt="vajra logo" width="200px" />
              </div>
            </div>
            <div className="col-6 col-md-2">
              <div className="d-flex flex-column justify-content-center align-items-center">
                <div className="mx-0">
                  <h4 className="text-primary noUnderline fs-4 lh-base ms-0">
                    About
                  </h4>
                </div>
                <div className="d-flex flex-column justify-content-center align-items-center">
                  <a href="/" className="text-muted noUnderline fs-4 lh-base">
                    Features
                  </a>
                  <a href="/" className="text-muted noUnderline fs-4 lh-base">
                    Compare
                  </a>
                  <a href="/" className="text-muted noUnderline fs-4 lh-base">
                    Support
                  </a>
                </div>
              </div>
            </div>
            <div className="col-6 col-md-2">
              <div className="d-flex flex-column justify-content-center align-items-center">
                <div className="mx-0">
                  <h4 className="text-primary fs-4 lh-base ms-0">Social</h4>
                </div>
                <div className="d-flex flex-column justify-content-center align-items-center">
                  <a href="/" className="text-muted noUnderline fs-4 lh-base">
                    Twitter
                  </a>
                  <a href="/" className="text-muted noUnderline fs-4 lh-base">
                    LinkedIn
                  </a>
                  <a href="/" className="text-muted noUnderline fs-4 lh-base">
                    Instagram
                  </a>
                </div>
              </div>
            </div>
          </div>
          <div className="p-3">
            <hr />
            <div className="d-flex pt-1 justify-content-between">
              <div className="text-size-small text-color-muted fs-4 lh-base">
                ©2023 Vajra EDR Tools.
              </div>
              <div className="footer-subnav">
                <a href="#" className="footer-subnav-link fs-4 lh-base">
                  Reach us
                </a>
                <a href="#" className="footer-subnav-link fs-4 lh-base">
                  Privacy Policy
                </a>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default HomePage;
