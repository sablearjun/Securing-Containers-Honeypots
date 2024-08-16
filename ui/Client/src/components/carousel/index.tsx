import { useState } from 'react';
import Carousel from 'react-bootstrap/Carousel';
import Image from 'react-bootstrap/Image';
import mainpagedashboard from "../../assets/images/mainpagedashboard.png";
import alertspage from "../../assets/images/alertspage.png";

const ControlledCarousel = () => {
  const [index, setIndex] = useState(0);

  const handleSelect = (selectedIndex: number) => {
    setIndex(selectedIndex);
  };

  return (

    <Carousel data-bs-theme="dark" activeIndex={index} onSelect={handleSelect}>
      <Carousel.Item>
        <Image src={mainpagedashboard} alt="Main Page Dashboard" fluid />
      </Carousel.Item>
      <Carousel.Item>
        <Image src={alertspage} alt="Alerts Page" fluid />
      </Carousel.Item>
    </Carousel>
  );
};

export default ControlledCarousel;
