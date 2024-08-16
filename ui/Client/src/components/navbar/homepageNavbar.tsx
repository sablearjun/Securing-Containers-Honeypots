import Container from 'react-bootstrap/Container';
import Nav from 'react-bootstrap/Nav';
import Navbar from 'react-bootstrap/Navbar';
import Logo from '../../assets/images/vajralogo.svg';

const HomePageNavbar = () =>{
  return (
<Navbar collapseOnSelect expand="md" className="bg-body-tertiary">
  <Container fluid className='p-2'>
    <Navbar.Brand href="/" className='ms-5'>
      <img alt="Vajra UI Logo" src={Logo} />
    </Navbar.Brand>
    <Navbar.Toggle aria-controls="responsive-navbar-nav" />
    <Navbar.Collapse id="responsive-navbar-nav">
      <Nav className="me-auto mx-auto">
        <Nav.Link href="/#about" className="p-3 fs-5">About</Nav.Link>
        <Nav.Link href="/#features" className="p-3 fs-5">Features</Nav.Link>
        <Nav.Link href="/#team" className="p-3 fs-5">Team</Nav.Link>
        <Nav.Link href="/bloglist" className="p-3 fs-5">Blogs</Nav.Link>
      </Nav>
      <Nav>
        <Nav.Link className="nav-link text-white bg-primary p-3 fs-5" href="/login">
          Login/Signup
        </Nav.Link>
      </Nav>
    </Navbar.Collapse>
  </Container>
</Navbar>

  );
}

export default HomePageNavbar;